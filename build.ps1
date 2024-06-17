param(
    [string]$buildType = "Debug"
)

$outputDir = "bin"

cmake -S . -B $outputDir/$buildType -G "Ninja" -DCMAKE_BUILD_TYPE="$buildType" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

cmake --build $outputDir/$buildType
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

$commandsPath = "$outputDir/$buildType/compile_commands.json"
$currPath = (Get-Location).Path

Move-Item -Path $commandsPath -Destination $currPath/compile_commands.json -Force
