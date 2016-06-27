Name:       swap-probe-total
Summary:    SWAP probe library
Version:    3.0
Release:    1
Group:      System/Libraries
License:    LGPL-2.1+ and MIT
Source:    %{name}_%{version}.tar.gz


# setup config
%define WAYLAND_SUPPORT 0

%if "%{_with_wayland}" == "1"
%define WAYLAND_SUPPORT 1
%endif # _with_wayland


# unsupport arch
ExcludeArch: aarch64 x86_64


# common requires
BuildRequires: ecore-devel
BuildRequires: elementary-devel
BuildRequires: capi-appfw-application-devel
BuildRequires: capi-system-runtime-info-devel
BuildRequires: python
BuildRequires: glibc-debuginfo


# Applied python acceleration
%ifarch armv7l
BuildRequires: python-accel-armv7l-cross-arm
%endif # arm7l

%ifarch aarch64
BuildRequires: python-accel-aarch64-cross-aarch64
%endif # aarch64

BuildRequires:  pkgconfig(sensor)

# graphic support
BuildRequires: pkgconfig(gles20)
BuildRequires: pkgconfig(wayland-egl)
BuildRequires: pkgconfig(egl)

Provides:  swap-probe


%description
SWAP probe is a part of data collection back-end for DA.
This library will be installed in target.

%package -n swap-probe
Provides: swap-probe
Summary:    SWAP probe library

%description  -n swap-probe
SWAP probe is a part of data collection back-end for DA.
This library will be installed in target.


%package -n swap-probe-devel
Provides: swap-probe-devel
Summary:    SWAP probe library

%description  -n swap-probe-devel
SWAP probe is a part of data collection back-end for DA.
This library will be installed in target.

%package -n swap-probe-elf
Provides: swap-probe-elf
Summary:    Swap elf parsing tool

%description -n swap-probe-elf
SWAP probe is a part of data collection back-end for DA.
This tool will be installed in target

%prep
%setup -q -n %{name}_%{version}

%build
export WAYLAND_SUPPORT=y

make rmheaders
make headers
make -j
make ldheader

%install
rm -rf ${RPM_BUILD_ROOT}
mkdir -p %{buildroot}/usr/share/license
cp LICENSE.MIT %{buildroot}/usr/share/license/%{name}
cat LICENSE.LGPLv2.1+ >> %{buildroot}/usr/share/license/%{name}

mkdir -p %{buildroot}/usr/local/include
mkdir -p %{buildroot}/usr/local/bin

%make_install

%files -n swap-probe
/usr/share/license/%{name}

%manifest swap-probe.manifest
%defattr(-,root,root,-)
%{_prefix}/lib/da_probe_tizen.so

%files -n swap-probe-devel
/usr/local/include/ld_preload_*.h
/usr/local/include/x_define_api_id_list.h
/usr/local/include/app_protocol.h
/usr/local/include/linker_info.h

%files -n swap-probe-elf
%manifest swap-probe-elf.manifest
%defattr(-,root,root,-)
/usr/local/bin/parse_elf
/usr/local/include/parse_elf.h
%{_prefix}/lib/libparserelf.so


%changelog
