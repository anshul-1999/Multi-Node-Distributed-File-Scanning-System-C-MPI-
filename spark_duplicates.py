from pyspark.sql import SparkSession
from pyspark.sql import functions as F
import sys

def main():
    if len(sys.argv) < 2:
        print("Usage: spark-submit spark_duplicates.py <input_csv> [output_dir]")
        sys.exit(1)

    input_csv = sys.argv[1]
    output_dir = sys.argv[2] if len(sys.argv) >= 3 else None

    spark = (
        SparkSession.builder
        .appName("DistributedDuplicateCandidatesBySize")
        .getOrCreate()
    )

    # Read CSV produced by sequential_scanner / parallel_scanner
    df = (
        spark.read
        .option("header", "true")
        .option("inferSchema", "true")
        .csv(input_csv)
        .select(
            F.col("size").cast("long").alias("size"),
            F.col("path").cast("string").alias("path")
        )
        .filter(F.col("size").isNotNull() & (F.col("size") > 0) & F.col("path").isNotNull())
    )

    # Distributed aggregation: group by size, collect paths, count
    grouped = (
        df.groupBy("size")
        .agg(
            F.count("*").alias("count"),
            F.collect_list("path").alias("paths")
        )
        .filter(F.col("count") > 1)
        .orderBy(F.desc("size"))
    )

    # Show a readable report in the console
    print("\n=== Spark Duplicate Candidates (size-based) ===")
    rows = grouped.take(50)  # limit console spam
    for r in rows:
        print(f"Size: {r['size']} bytes | count: {r['count']}")
        for p in r["paths"][:20]:  # limit paths printed per size
            print(f"  -> {p}")
        if len(r["paths"]) > 20:
            print(f"  ... (+{len(r['paths']) - 20} more)")
        print()

    # Optionally write results for your demo/report
    if output_dir:
        (
            grouped
            .withColumn("paths_joined", F.concat_ws(" | ", F.col("paths")))
            .select("size", "count", "paths_joined")
            .coalesce(1)  # easier to find output file for demo
            .write
            .mode("overwrite")
            .option("header", "true")
            .csv(output_dir)
        )
        print(f"[+] Wrote distributed results to: {output_dir}")

    spark.stop()

if __name__ == "__main__":
    main()