#/bin/bash
# Check the iwyu_mapping.imp file against codebase to identify symbols we may want to define for IWYU

MAPPING_FILE=iwyu_mapping.imp
_source=.iwyu.source.tmp
_sym_tmp=.iwyu.symbol.tmp
_inc_tmp=.iwyu.include.tmp
_not_defined_yet=.iwyu.strings.tmp
_defined_already=.iwyu.defined.tmp
_errors=.iwyu.errors.tmp

# get the symbols and includes we've already defined
grep -oP "(?<=\[\").*?(?=\")" $MAPPING_FILE > $_defined_already

# extract the hard-coded symbols and includes from IWYU source that we haven't already defined, and save as $_not_defined_yet
curl -s https://raw.githubusercontent.com/include-what-you-use/include-what-you-use/master/iwyu_include_picker.cc > $_source
sed -n '/IncludeMapEntry libc_symbol_map/,/};/{p;/^};/q}' $_source > $_sym_tmp
sed -n '/IncludeMapEntry libc_include_map/,/};/{p;/^};/q}' $_source > $_inc_tmp
egrep -v "(//)" $_sym_tmp $_inc_tmp | grep -oP "(?<=\{ \")(.*?)(?=\")" | grep -v -Fwf $_defined_already | uniq > $_not_defined_yet

# find lines in our code that have symbols and includes that are not defined yet
find . -type f -name "*.c" -o -name "*.h" -exec grep -n -Fwf $_not_defined_yet '{}' /dev/null \; > $_errors

echo "# IWYU **********************"
echo "# Symbols/Includes in the following code overlap with IWYU defaults"
echo "# Consider defining them in iwyu_mapping.imp\n"
cat $_errors

# clean up
rm $_source
rm $_sym_tmp
rm $_inc_tmp
rm $_not_defined_yet
# rm $_defined_already
rm $_errors