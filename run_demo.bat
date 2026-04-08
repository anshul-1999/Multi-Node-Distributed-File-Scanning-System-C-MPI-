@echo off
setlocal enabledelayedexpansion

echo ====================================================
echo   "A&N Parallel Dynamics - Clean Demo"
echo ====================================================

:: 1) Create Test Data (smaller dataset for fast demo)
echo [+] Creating test directories...
mkdir data1 data2 data3 data4 2>nul

echo [+] Cleaning old test files...
del /q data1\* data2\* data3\* data4\* 2>nul
del /q seq_meta.csv par_meta.csv 2>nul

echo [+] Generating demo files...
set COUNT=200

for /L %%i in (1,1,%COUNT%) do (
  echo file %%i content %%i > data1\f_%%i.txt
  echo file %%i content %%i > data2\f_%%i.txt
)

:: Create duplicates
echo duplicate content line > data3\dup_a.txt
echo duplicate content line > data4\dup_b.txt
echo duplicate content line > data1\dup_c.txt
echo duplicate content line > data2\dup_d.txt

:: Larger duplicate files
del /q data3\big1.txt data4\big2.txt 2>nul
for /L %%i in (1,1,50) do (
  echo This is a larger file line %%i>> data3\big1.txt
  echo This is a larger file line %%i>> data4\big2.txt
)

:: 2) Compile
echo.
echo [+] Compiling Sequential version...
g++ sequential_scanner.cpp -O2 -o sequential_scanner.exe
if %errorlevel% neq 0 (
  echo [!] Sequential compilation failed.
  pause
  exit /b
)

echo [+] Compiling Parallel version (OpenMP)...
g++ parallel_scanner.cpp -O2 -fopenmp -o parallel_scanner.exe
if %errorlevel% neq 0 (
  echo [!] Parallel compilation failed.
  pause
  exit /b
)

:: 3) Run Sequential
echo.
echo ==============================
echo RUNNING: Sequential Baseline
echo ==============================
sequential_scanner.exe --csv seq_meta.csv
echo.
pause

:: 4) Run Parallel (4 threads only)
echo.
echo ==============================
echo RUNNING: Parallel OpenMP (4 threads)
echo ==============================
set OMP_NUM_THREADS=4
parallel_scanner.exe --csv par_meta.csv
echo.
pause

echo Demo completed.
endlocal