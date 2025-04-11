# Maple Windows Installer Script 🚀
# Version to install
$version = "1.1"

# Update these URLs with the actual release links from your GitHub repository
$mapleUrl = "https://github.com/ItsCodey1095/Maple/releases/download/v$version/maple.exe"
$mvmUrl = "https://github.com/ItsCodey1095/Maple/releases/download/v$version/mvm.exe"

# Installation directory
$installDir = "C:\Maple"

# Create the directory if it doesn't exist.
if (!(Test-Path $installDir)) {
    Write-Output "Creating installation directory at $installDir..."
    New-Item -ItemType Directory -Path $installDir
}

Write-Output "Downloading maple.exe 😎..."
Invoke-WebRequest -Uri $mapleUrl -OutFile "$installDir\maple.exe"

Write-Output "Downloading mvm.exe 🚀..."
Invoke-WebRequest -Uri $mvmUrl -OutFile "$installDir\mvm.exe"

Write-Output "Installation complete! 🎉"
Write-Output "Don't forget to add $installDir to your PATH environment variable if it's not already there."

# Optional: Add install directory to current session PATH for immediate use.
$env:Path += ";$installDir"
