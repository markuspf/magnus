#!/usr/bin/wish -f

cd tex
set files [glob *.dvi]
foreach fileName $files {
    eval exec dvips -o $fileName.ps $fileName
}

exit