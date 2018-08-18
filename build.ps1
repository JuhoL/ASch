if ($Args[1] -Match "clean")
{
    $cleanFlag = "-c"
}

if ($Args[0] -Match "queue")
{
    scons test=ASch_Queue $cleanFlag
    if ($Args[1] -NotMatch "clean")
    {
        $runtest = ./Build/Tests/UTest_ASch_Queue.exe
        $runcoverage = gcovr -r . --filter='(.*\/sources\/ASch_Queue.*cpp\b)' --html --html-details -o queue.html
    }
}
else
{
    scons
}

if ($Args[1] -NotMatch "clean")
{
    "Running unit tests..."
    $runtest
    $runcoverage
}
