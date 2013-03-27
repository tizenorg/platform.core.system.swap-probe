Name:       dynamic-analysis-probe
Summary:    dynamic analyzer probe library
Version:    2.1.0
Release:    1
Group:      System/Libraries
License:	GNU Lesser General Public License, Version 2.1
Source:    %{name}_%{version}.tar.gz
BuildRequires:  osp-appfw-devel
BuildRequires:  osp-uifw-devel
BuildRequires:  ecore-input-evas
BuildRequires:  elementary-devel
BuildRequires:  capi-appfw-application-devel
BuildRequires:  capi-system-runtime-info-devel
BuildRequires:  libXext-devel
#BuildRequires:  libX11-devel
#BuildRequires:  evas-devel


%description
Dynamic analysis probe is for dynamic analyzer.
This library will be installed in target.

%prep
%setup -q -n %{name}_%{version}

%build
make

%install
rm -rf ${RPM_BUILD_ROOT}
%make_install

%files
%defattr(-,root,root,-)
%{_prefix}/lib/da_probe_osp.so
%{_prefix}/lib/libdaprobe.so


%changelog

