#!/bin/sh

# ${1} is the directory containing the .gcno files (%{buildDir} in Qt Creator)

LCOV=lcov
GENHTML=genhtml
#BROWSER="/usr/bin/opera"

SRC_DIR="."
HTML_RESULTS="html"

mkdir -p ${HTML_RESULTS}

"${LCOV}" -c -i -d "${SRC_DIR}" -o "${SRC_DIR}/app_base.info"

./appTest/appTest
# generate our initial info
"${LCOV}" -d "${SRC_DIR}" -c -o "run.info"

"${LCOV}" -a "${SRC_DIR}/app_base.info" -a "${SRC_DIR}/run.info" -o "${SRC_DIR}/coverage.info"
# remove some paths
"${LCOV}" -r "${SRC_DIR}/coverage.info" "*.h" "*test*" "*include/Qt*.h" "*/include/c++/*" "*Qt*.framework*" "*Xcode.app*" "*.moc" "*moc_*.cpp" "*/test/*" -o "${SRC_DIR}/coverage-filtered.info"

# generate our HTML
"${GENHTML}" --legend --branch-coverage --function-coverage --demangle-cpp -o "${HTML_RESULTS}" "${SRC_DIR}/coverage-filtered.info"

# reset our counts
"${LCOV}" -d "${SRC_DIR}" -z

# open in browser and bring to front
#"${BROWSER}" "${HTML_RESULTS}/index.html"
#open "${BROWSER}"
