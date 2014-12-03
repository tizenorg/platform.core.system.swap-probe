Name:       swap-probe-total
Summary:    SWAP probe library
Version:    3.0
Release:    1
Group:      System/Libraries
License:	GNU Lesser General Public License, Version 2.1
Source:    %{name}_%{version}.tar.gz
BuildRequires:  ecore-input-evas
BuildRequires:  elementary-devel
BuildRequires:  capi-appfw-application-devel
BuildRequires:  capi-system-runtime-info-devel
BuildRequires:  libXext-devel
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

%prep
%setup -q -n %{name}_%{version}

%build
make rmheaders
make headers
make -j
make ldheader

%install
rm -rf ${RPM_BUILD_ROOT}
mkdir -p %{buildroot}/usr/share/license
cp LICENSE %{buildroot}/usr/share/license/%{name}
mkdir -p %{buildroot}/usr/local/include

%make_install

%files -n swap-probe
/usr/share/license/%{name}

%manifest swap-probe.manifest
%defattr(-,root,root,-)
%{_prefix}/lib/da_probe_tizen.so
%{_prefix}/lib/libdaprobe.so
%{_prefix}/lib/da_api_map

%files -n swap-probe-devel
/usr/local/include/ld_preload_*.h

%changelog

