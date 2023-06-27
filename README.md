[![cpp-logo](https://img.shields.io/badge/C++v14-Solutions-blue.svg?style=flat&logo=c%2B%2B)](
https://en.wikipedia.org/wiki/C++
)

[![Run tests, then collect result and coverage](https://github.com/Alex0vSky/turbo-octo-adventure/actions/workflows/TestsResultAndCoverage.yml/badge.svg)](https://github.com/Alex0vSky/turbo-octo-adventure/actions/workflows/TestsResultAndCoverage.yml)

[![cpplint](https://gist.githubusercontent.com/Alex0vSky/2af621bdd237231125e907ea81b1f8a8/raw/GoogleStyle_cpplint.svg)](
https://Alex0vSky.github.io/project-qa-report/TinySynapticsScroll/cpplint.xml
)
[![cppcheck](https://gist.githubusercontent.com/Alex0vSky/2af621bdd237231125e907ea81b1f8a8/raw/StaticAnalysis_cppcheck.svg)](
https://Alex0vSky.github.io/project-qa-report/TinySynapticsScroll/cppcheck.xml
)

[![LinesOfСode](
https://gist.githubusercontent.com/Alex0vSky/2af621bdd237231125e907ea81b1f8a8/raw/Metrixpp-LinesOfСode.svg
)](
https://Alex0vSky.github.io/project-qa-report/TinySynapticsScroll/metrixpp.txt
)
[![Comments](
https://gist.githubusercontent.com/Alex0vSky/2af621bdd237231125e907ea81b1f8a8/raw/Metrixpp-Comments.svg
)](
https://Alex0vSky.github.io/project-qa-report/TinySynapticsScroll/metrixpp.txt
)

[![amount testsuites Windows-x86-Debug](
https://gist.githubusercontent.com/Alex0vSky/2af621bdd237231125e907ea81b1f8a8/raw/GoogleTest-testsuites-Windows-x86-Debug.svg
)](
https://Alex0vSky.github.io/project-qa-report/TinySynapticsScroll/GoogleTestCombinedOutput/GoogleTestCombinedOutput.html
)
[![tests coverage Windows-x86-Debug](
https://gist.githubusercontent.com/Alex0vSky/2af621bdd237231125e907ea81b1f8a8/raw/TestsCoverage-Occ-Windows-x86-Debug.svg
)](
https://Alex0vSky.github.io/project-qa-report/TinySynapticsScroll/HtmlReportOcc/index.html
)

# TinySynapticsScroll ![icon1](https://github.com/Alex0vSky/TinySynapticsScroll/assets/52796897/ba4cfb9b-0568-4221-a64d-4eba135c9bca)
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

The testing process is implemented through CI gitgub actions.
Code verification aka StaticAnalysis and code metrics is implemented through CI gitgub actions.

CI gitgub actions uses:
running tests based on google-test, a modified version of the smalti/gmock-win32 library is also used;
releases of OpenCppCoverage/OpenCppCoverage to get code coverage with tests, in the form of html;
py junitparser to combine junit test reports into one file;
fork repo burlachenkok/gtest_report updated to work with Python3 to get html from junit reports;
py metrixpp to get code metrics;
cpplint releases to bring code style closer to GoogleStyle;
cppcheck releases to statically analyze the code for errors.

Building the project and tests from the source code requires the presence of the Microsoft Visual Studio 2019 Community, and using *.sln and *.vcxproj project files.
