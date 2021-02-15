#/bin/bash
# Check the iwyu_mapping.imp file against codebase to identify symbols we may want to define for IWYU

if [ $# -lt 2 ]; then
    echo "iwyu_map_check.sh"
    echo "Compare your source code to your iwyu_mapping.imp file, and highlight any symbols that we may want to define."
    echo "An updated map file keeps IWYU happy and avoid false positives."
    echo ""
    echo "Usage:"
    echo "  map_check.sh <source file directory> <map_file>"
    echo ""
    echo "  For example:"
    echo "     \$iwyu_map_check.sh ../kernel ../kernel/iwyu_mapping.imp"
    exit 1
fi

if [ ! -f $2 ]; then
    echo "Map file $2 not found!"
    exit 1
fi

MAPPING_FILE=$2
_source=$(mktemp)
_sym_tmp=$(mktemp)
_inc_tmp=$(mktemp)
_not_defined_yet=$(mktemp)
_defined_already=$(mktemp)
_errors=$(mktemp)

# get the symbols and includes we've already defined
grep -oP "(?<=\[\").*?(?=\")" $MAPPING_FILE > $_defined_already

# extract the hard-coded symbols and includes from IWYU source that we haven't already defined, and save as $_not_defined_yet
curl -s https://raw.githubusercontent.com/include-what-you-use/include-what-you-use/master/iwyu_include_picker.cc > $_source
sed -n '/IncludeMapEntry libc_symbol_map/,/};/{p;/^};/q}' $_source > $_sym_tmp
sed -n '/IncludeMapEntry libc_include_map/,/};/{p;/^};/q}' $_source > $_inc_tmp
egrep -v "(//)" $_sym_tmp $_inc_tmp | grep -oP "(?<=\{ \")(.*?)(?=\")" | grep -v -Fwf $_defined_already | uniq > $_not_defined_yet

# find lines in our code that have symbols and includes that are not defined yet (ignoring comments)
find $1 -type f -name "*.c" -o -name "*.h" -exec sh -c "sed 's|[[:blank:]]*//.*||;' {} | grep --label={} -H -n -Fwf $_not_defined_yet -" \; > $_errors

# clean up
rm $_source
rm $_sym_tmp
rm $_inc_tmp
rm $_not_defined_yet
rm $_defined_already

if [ -s $_errors ]
then 
    echo ""
    echo "# iwyu_map_check"
    echo "# Symbols/Includes in the following code overlap with IWYU defaults"
    echo "# Consider defining them in iwyu_mapping.imp\n"
    cat $_errors
    exit 1
else
    rm $_errors
    echo ""
    echo "# iwyu_map_check"
    echo "No issues."
    exit 0
fi
