usage () {
  echo "Usage: $0 <sources_dir> <headers_dir>"
  echo ""
  echo "Checks that all the source files in <sources_dir> have a corresponding"
  echo "header file in <headers_dir> and vice versa."
  echo ""
}

if [ $# -ne 2 ]; then
  usage
  exit 1
fi

sources_dir=$1
headers_dir=$2

if [ ! -d "$sources_dir" ]; then
  echo "Error: $sources_dir is not a directory"
  exit 1
fi

if [ ! -d "$headers_dir" ]; then
  echo "Error: $headers_dir is not a directory"
  exit 1
fi

# Get the list of source files (cpp)
sources=$(find "$sources_dir" -name "*.cpp" | sort)

# Get the list of header files (h)
headers=$(find "$headers_dir" -name "*.h" | sort)

# Get the list of source files without the directory prefix
sources_basename=$(echo "$sources" | sed "s|$sources_dir/||" | sort)

# Get the list of header files without the directory prefix
headers_basename=$(echo "$headers" | sed "s|$headers_dir/||" | sort)

# Get the list of source files without the .cpp extension
sources_noext=$(echo "$sources_basename" | sed "s|\.cpp$||" | sort)

# Get the list of header files without the .h extension
headers_noext=$(echo "$headers_basename" | sed "s|\.h$||" | sort)

# Get the list of source files without the directory prefix and .cpp extension
sources_noext_basename=$(echo "$sources" | sed "s|$sources_dir/||" | sed "s|\.cpp$||" | sort)

# Get the list of header files without the directory prefix and .h extension
headers_noext_basename=$(echo "$headers" | sed "s|$headers_dir/||" | sed "s|\.h$||" | sort)

# Get the list of source files that don't have a corresponding header file
sources_no_header=$(comm -23 <(echo "$sources_noext_basename") <(echo "$headers_noext_basename") | sort)

# Get the list of header files that don't have a corresponding source file
headers_no_source=$(comm -23 <(echo "$headers_noext_basename") <(echo "$sources_noext_basename") | sort)


# Check that all the source files have a corresponding header file
if [ -n "$sources_no_header" ]; then
  echo "Error: The following source files don't have a corresponding header file:"
  echo "$sources_no_header"
fi

# Check that all the header files have a corresponding source file
if [ -n "$headers_no_source" ]; then
  echo "Error: The following header files don't have a corresponding source file:"
  echo "$headers_no_source"
fi