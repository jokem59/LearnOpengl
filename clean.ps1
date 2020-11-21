Param(
    [switch]$build = $false,
    [switch]$bin = $false
)

if ($build -eq $true)
{
    Remove-Item $PSScriptRoot/build/* -Recurse;
}

if ($bin -eq $true)
{
    Remote-Item $PSScriptRoot/bin/* -Recurse;
}
