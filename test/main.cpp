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

#define DOCTEST_CONFIG_IMPLEMENT
#define DOCTEST_CONFIG_NO_TRY_CATCH_IN_ASSERTS
#define DOCTEST_CONFIG_SUPER_FAST_ASSERTS
#include "doctest.h"
#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP

using namespace std;

void init_stuff() {
    ios::sync_with_stdio(false);

    el::Configurations defaultConf;
    defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime %level: %msg");
    el::Loggers::reconfigureAllLoggers(defaultConf);
}

int main(int argc, char** argv) {
    doctest::Context context; // initialize
    init_stuff();

    // defaults
    context.setOption("no-breaks", false);             // don't break in the debugger when assertions fail

    context.applyCommandLine(argc, argv);

    // overrides
    context.setOption("abort-after", 5); // stop test execution after 5 failed assertions
    context.setOption("sort", "name");   // sort the test cases by their name

    int res = context.run(); // run

    if(context.shouldExit()) // important - query flags (and --exit) rely on the user doing this
        return res;          // propagate the result of the tests

    int client_stuff_return_code = 0;
    // your program - if the testing framework is integrated in your production code

    return res + client_stuff_return_code; // the result from doctest is propagated here as well
}