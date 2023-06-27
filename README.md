[![cpp-logo](https://img.shields.io/badge/C++v14-Solutions-blue.svg?style=flat&logo=c%2B%2B)](
https://en.wikipedia.org/wiki/C++
)

[![Run tests, then collect result and coverage](https://github.com/Alex0vSky/turbo-octo-adventure/actions/workflows/TestsResultAndCoverage.yml/badge.svg)](https://github.com/Alex0vSky/turbo-octo-adventure/actions/workflows/TestsResultAndCoverage.yml)

[![cpplint](https://gist.githubusercontent.com/Alex0vSky/2af621bdd237231125e907ea81b1f8a8/raw/GoogleStyle_cpplint.svg)](
https://Alex0vSky.github.io/sturdy-octo-disco/cpplint.xml
)
[![cppcheck](https://gist.githubusercontent.com/Alex0vSky/2af621bdd237231125e907ea81b1f8a8/raw/StaticAnalysis_cppcheck.svg)](
https://Alex0vSky.github.io/sturdy-octo-disco/cppcheck.xml
)

[![LinesOfСode](
https://gist.githubusercontent.com/Alex0vSky/2af621bdd237231125e907ea81b1f8a8/raw/Metrixpp-LinesOfСode.svg
)](
https://Alex0vSky.github.io/sturdy-octo-disco/metrixpp.txt
)
[![Comments](
https://gist.githubusercontent.com/Alex0vSky/2af621bdd237231125e907ea81b1f8a8/raw/Metrixpp-Comments.svg
)](
https://Alex0vSky.github.io/sturdy-octo-disco/metrixpp.txt
)

[![amount testsuites Windows-x86-Debug](
https://gist.githubusercontent.com/Alex0vSky/2af621bdd237231125e907ea81b1f8a8/raw/GoogleTest-testsuites-Windows-x86-Debug.svg
)](
https://Alex0vSky.github.io/sturdy-octo-disco/GoogleTestCombinedOutput/GoogleTestCombinedOutput.html
)
[![tests coverage Windows-x86-Debug](
https://gist.githubusercontent.com/Alex0vSky/2af621bdd237231125e907ea81b1f8a8/raw/TestsCoverage-Occ-Windows-x86-Debug.svg
)](
https://Alex0vSky.github.io/sturdy-octo-disco/HtmlReportOcc/index.html
)

# TinySynapticsScroll
The application allows you to use two-finger scrolling on the Synaptics touchpad.
Requires installed touchpad drivers and SynCOM.dll library.

Publication of a home pet project.
Written in order to minimize the amount of memory consumed by the process.
The scroll function code is taken from the https://github.com/awahlig/two-finger-scroll repo.
Application and tray icon taken from here https://icon-icons.com/download/57949/ICO/32/ and modified.

This application is a permanently resident program with the ability to control through the system tray (where the clock is).
Inside it is a COM client interacting with the SynCOM.dll library.

The application consists of one file. Does not require installation. The application saves settings in the registry. 
In the settings you can specify autorun with Windows.
In the settings, you can adjust the speed and acceleration of scrolling.
