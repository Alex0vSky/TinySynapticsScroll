//
// pch.h
//

#pragma once

#include "gtest\gtest.h"

#include "gmock\gmock.h"
#include "gmock-win32.h"
#include "extension-gmock-win32.h"

// @insp https://developercommunity.visualstudio.com/t/test-explorer-namespace-seems-to-not-be-working/663974
// @insp https://bitbucket.org/CadActive/workspace/snippets/GrBakB/macro-to-enable-namespaces-in-google-test
#include "ThirdParty\vs_google_test_explorer_namespace.h"

#include "..\..\src\MsvcGenerated\stdafx.h"
