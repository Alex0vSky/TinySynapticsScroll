# ![forGitHubReadme_icon1](https://github.com/Alex0vSky/TinySynapticsScroll/assets/52796897/e0a3428d-4577-4461-bc1b-22525ecf3595) _TinySynapticsScroll_

[![windows-os](https://img.shields.io/badge/platform-windows-windows?logo=Windows&color=blue)](
https://en.wikipedia.org/wiki/Microsoft_Windows
) [![cpp-logo](https://img.shields.io/badge/C%2B%2B-v14-blue?logo=cplusplus)](
https://en.wikipedia.org/wiki/C++
)

[![Run tests, then collect result and coverage](https://github.com/Alex0vSky/TinySynapticsScroll/actions/workflows/TestsResultAndCoverage.yml/badge.svg)](https://github.com/Alex0vSky/TinySynapticsScroll/actions/workflows/TestsResultAndCoverage.yml)

[![cpplint](https://gist.githubusercontent.com/Alex0vSky/2af621bdd237231125e907ea81b1f8a8/raw/GoogleStyle_cpplint.svg)](
https://Alex0vSky.github.io/project-qa-report/TinySynapticsScroll/cpplint.xml
) [![cppcheck](https://gist.githubusercontent.com/Alex0vSky/2af621bdd237231125e907ea81b1f8a8/raw/StaticAnalysis_cppcheck.svg)](
https://Alex0vSky.github.io/project-qa-report/TinySynapticsScroll/cppcheck.xml
) [![LinesOfСode](https://gist.githubusercontent.com/Alex0vSky/2af621bdd237231125e907ea81b1f8a8/raw/Metrixpp-LinesOfСode.svg)](
https://Alex0vSky.github.io/project-qa-report/TinySynapticsScroll/metrixpp.txt
) [![Comments](https://gist.githubusercontent.com/Alex0vSky/2af621bdd237231125e907ea81b1f8a8/raw/Metrixpp-Comments.svg)](
https://Alex0vSky.github.io/project-qa-report/TinySynapticsScroll/metrixpp.txt
) [![amount testsuites](https://gist.githubusercontent.com/Alex0vSky/2af621bdd237231125e907ea81b1f8a8/raw/GoogleTest-testsuites-Windows-x86-Debug.svg)](
https://Alex0vSky.github.io/project-qa-report/TinySynapticsScroll/GoogleTestCombinedOutput/GoogleTestCombinedOutput.html
) [![tests coverage](https://gist.githubusercontent.com/Alex0vSky/2af621bdd237231125e907ea81b1f8a8/raw/TestsCoverage-Occ-Windows-x86-Debug.svg)](
https://Alex0vSky.github.io/project-qa-report/TinySynapticsScroll/HtmlReportOcc/index.html
)

The application allows you to use two-finger scrolling on __the Synaptics touchpad__.

## Features
Written in order to minimize the amount of memory consumed by the process. The process consumes memory from 250 to 500 kilobytes.
> Publication of a home pet project.

## Requirements
Requires installed touchpad drivers and SynCOM.dll library.

## Install
The application consists of one file. Does not require installation. 

## Usage
This application is a permanently resident program with the ability to control through the system tray (where the clock is).
The application saves settings in the registry. 
In the settings you can specify autorun with ___Windows___.
In the settings, you can adjust the speed and acceleration of scrolling.
Inside it is a __COM client__ interacting with the __SynCOM.dll__ library. 

## Tests
The testing process is implemented through CI github actions.
Code verification aka StaticAnalysis and code metrics is implemented through CI github actions.
### CI github
CI github actions uses:
+ running tests based on _google-test_, a modified version of the _smalti/gmock-win32_ library is also used;
+ _OpenCppCoverage/OpenCppCoverage_ to get code coverage with tests, in the form of html;
+ py _junitparser_ to combine junit test reports into one file;
+ fork repo _burlachenkok/gtest_report_ updated to work with Python3 to get html from junit reports;
+ py _metrixpp_ to get code metrics;
+ _cpplint_ to bring code style closer to GoogleStyle;
+ _cppcheck_ to statically analyze the code for errors.

## Build
Building the project and tests from the source code requires the presence of the __Microsoft Visual Studio 2019 Community__, and using *.sln and *.vcxproj project files. The project is also ready to be built via __clang__ version __12.0.0__, which comes with __VIsualStudioCommunity2019__.

## Contributing
Can ask questions. PRs are accepted. No requirements for contributing.

## Thanks
The scroll function code is taken from the [awahlig/two-finger-scroll](https://github.com/awahlig/two-finger-scroll) repo.
Application and tray icon taken from here [icon-icons.com](https://icon-icons.com/download/57949/ICO/32/) and modified.

## License
See the [LICENSE](https://github.com/Alex0vSky/TinySynapticsScroll/blob/main/LICENSE) file for license rights and limitations (MIT).
