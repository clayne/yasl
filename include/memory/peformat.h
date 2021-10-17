/*
  Copyright (c) 2021 Augusto Goulart
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#pragma once

#include "base.h"

namespace Memory
{

class PEFormat {
public:
  PEFormat(const hmodule_t& hmodule) : _module(hmodule)
  {
    _filename = make_unique<char[]>(MAX_PATH); // MapAndLoad doesn't support wchar_t
    if (!GetModuleFileNameA(hmodule, &_filename[0], MAX_PATH))
      throw runtime_error(_STRCAT(__FUNCSIG__, "\tUnable to find process filename"));

    _image = make_unique<peimage_t>();
    if (!MapAndLoad(&_filename[0], NULL, &*_image, FALSE, TRUE))
      throw runtime_error(_STRCAT(__FUNCSIG__, "\tCould not map process binary file"));
  }

  ~PEFormat()
  {
    UnMapAndLoad(&*_image);
  }

  constexpr uintptr_t GetEntry() noexcept
  {
    return GetAbsolute(_image->FileHeader->OptionalHeader.AddressOfEntryPoint);
  }

private:
  hmodule_t             _module;
  unique_ptr<char[]>    _filename;
  unique_ptr<peimage_t> _image;
};

inline int32_t GetRelativeOffset(uintptr_t dest, uintptr_t from) noexcept
{
  return static_cast<int32_t>(dest - from);
};

inline uintptr_t GetAbsolute(uintptr_t address) noexcept
{
  return address + _BASE_ADDRESS;
};

}
