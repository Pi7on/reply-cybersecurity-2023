$basePath = "c:\Users\Marco\Desktop\ctf_2023\coding_100\l3"
$level = 6  # Starting level

while ($true) {
    $txtFile = Join-Path -Path $basePath -ChildPath ("lvl_$level.txt")
    $zipFile = Join-Path -Path $basePath -ChildPath ("lvl_$level.zip")

    # Break if the files don't exist
    if (-not (Test-Path -Path $txtFile) -or -not (Test-Path -Path $zipFile)) {
        break
    }

    # Get password from the main2.exe execution
    $password = & "C:\Users\Marco\Desktop\ctf_2023\coding_100\main2.exe" $txtFile
    Write-Host $password



    # Unzip using 7z and the obtained password
    & "7z" x $zipFile -p"$password"  -o"$basePath"

    $level++
}

Write-Host "Finished processing levels up to lvl_$($level - 1)"
