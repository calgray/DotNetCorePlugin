#
#  Copyright (c) Hubert Jarosz. All rights reserved.
#  Licensed under the MIT license. See LICENSE file in the project root for full license information.
#

CXXFLAGS = -Wall -Wextra -Werror -std=c++17
LDLIBS = -ldl -lcoreclr

.PHONY: all clean

all: SCCH Managed.dll Makefile

SCCH: simpleCoreCLRHost.cpp simpleCoreCLRHost.hpp utils.hpp Makefile
	git -C dynamicLinker pull || git clone https://github.com/Marqin/dynamicLinker
	make -C dynamicLinker CXX=$(CXX)
	$(CXX) $(CXXFLAGS) simpleCoreCLRHost.cpp -o SCCH -LdynamicLinker/ -ldynamicLinker $(LDLIBS)

Managed.dll: Managed/Managed.cs Makefile
	cd Managed && dotnet build
	cp Managed/bin/Debug/netstandard2.0/Managed.dll .

clean:
	rm -rf SCCH SCCH.dSYM Managed.dll
	sh -c "stat dynamicLinker/ &> /dev/null && make -C dynamicLinker clean" || true

distclean: clean
	rm -rf dynamicLinker/
