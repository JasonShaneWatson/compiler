Edit src/stage0.cpp

Compile with `make`
Test with `make test`
Clean builds directory with `make clean`

`make test` will display a table of tested programs and their comparison to
available obj and lst files; the first line of each file will be ignored, as
a diff is always expected on "Your Name(s)." A log file will also be created
as build/test.log, containing all diffs. Read the log with 
`less -r build/test.log`.


