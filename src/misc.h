/*
    Realm of Aesir backend
    Copyright (C) 2016  Michael de Lang

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <initializer_list>
#include <chrono>

using namespace std;

template<typename U, typename ... T>
bool one_of(U&& u, T && ... t)
{
    return ( (u == t) || ...  );
}

time_t get_current_time() noexcept {
    chrono::system_clock::time_point now = chrono::system_clock::now();
    return chrono::system_clock::to_time_t(now);
}