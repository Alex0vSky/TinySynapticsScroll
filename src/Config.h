// src\Config.h - config constants
#pragma once // Copyright 2023 Alex0vSky (https://github.com/Alex0vSky)
namespace prj_sysw { namespace TinySynapticsScroll { 
struct Config {
	struct detail_ {
		struct Switcher {
			static const DWORD c_dwOff = 0;
			static const DWORD c_dwOn = 1;
		};
		struct RegistryValueName {
			const wchar_t *c_wcsAccelerationValue = L"scrollAcc";
			const wchar_t *c_wcsAccelerationEnable = L"scrollAccEnabled";
			const wchar_t *c_wcsLinearEdgeEnable = L"scrollLinearEdge";
			const wchar_t *c_wcsMode = L"scrollMode";
			const wchar_t *c_wcsSpeedValue = L"scrollSpeed";
		};
	}; // struct detail_ _
	struct Movement {
		// Config location in registry 
		static const Tool::Registry::EnuRootKey s_enuRootKey = Tool::Registry::EnuRootKey::HKCU;
		static const wchar_t *getRegSubkey() {
			static const wchar_t *s_wcsSubKey = L"Software\\TinySynapticsScroll";
			return s_wcsSubKey;
		}
		struct Mode {
			static const DWORD c_dwCompatible = 0;
			static const DWORD c_dwSmooth = 1;
		};
		struct DefaultSettings {
			// Acceleration scroll, from 0 to 100
			static const DWORD c_dwAccelerationValue = 75;
			// Acceleration on/off, bool
			static const DWORD c_dwAccelerationEnable = detail_::Switcher::c_dwOn;
			// LinearEdge on/off, bool
			static const DWORD c_dwLinearEdgeEnable = detail_::Switcher::c_dwOn;
			// Scroll mode: 0 = "Compatible"; 1 = "Smooth"; others = "Smart".
			static const DWORD c_dwMode = Mode::c_dwSmooth;
			// Speed scroll, from 0 to 100
			static const DWORD c_dwSpeedValue = 100;
		};
	}; // struct Movement _
	struct Autorun {
		// Config location in registry 
		static const Tool::Registry::EnuRootKey s_enuRootKey = Tool::Registry::EnuRootKey::HKCU;
		static const wchar_t *getRegSubkey() {
			static const wchar_t s_wcsSubKey[] = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
			return s_wcsSubKey;
		}
		static const wchar_t *getRegValueName() {
			static const wchar_t s_wcsValueName[] = L"TinySynapticsScroll";
			return s_wcsValueName;
		}
	}; // struct Autorun _
};
}} // namespace prj_sysw::TinySynapticsScroll _
