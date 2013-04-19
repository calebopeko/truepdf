#include "options.h"

#include <cstdlib>
#include <sstream>
#include <iostream>
#include <fstream>

void Options::init()
{
  addParam("width", "300", "Set the window width.", 'w');
  addParam("height", "500", "Set the window height.", 'h');
  addParam("file", "", "Set the document to load.", 'f');
  addParam("verbose", "0", "Set verbosity.", 'v');
  addParam("preRender", "0", "Prerender all pages.", 'p');
  addParam("color", "default", "Apply color scheme. Values are 'default', 'dark', 'light'.", 'c');
}

void Options::addParam(const std::string& name, const std::string& initial, const std::string& description, char abbrev)
{
  addParam(OptionValue(name, initial, description, abbrev));
}

void Options::addParam(const OptionValue& v)
{
  if ( v.abbrev != 0 ) {
    if ( expand.find(v.abbrev) != expand.end() ) {
      std::cerr << "Warning: Overwriting abbreviation '" << v.abbrev << "' from '" << expand[v.abbrev] << "' to '" << v.name << "'!" << std::endl;
    }
    expand[v.abbrev] = v.name;
  }
  values[v.name] = v;
}

void Options::printUsage()
{
  std::cout << "Usage:" << std::endl;
  for ( std::map<std::string, OptionValue>::const_iterator iter = values.begin(); iter != values.end(); ++iter ) {
    std::cout << "   " << iter->second << std::endl;
  }
}

Options::Options()
{
  init();
}

Options::Options(int argc, char** argv)
{
  init();
  parse(argc, argv);
}

void Options::dump(const std::string& filename)
{
  std::ofstream file(filename.c_str());
  if ( file ) {
    for ( std::map<std::string, OptionValue>::const_iterator iter = values.begin(); iter != values.end(); ++iter ) {
      file << iter->first << ":\t" << iter->second.value << std::endl;
    }
  }
}

void Options::parse(int argc, char** argv)
{
  if ( argc == 2 ) {
    std::string value(argv[1]);
    if ( value[0] != '-' ) {
      values["file"].value = value;
      return;
    } else {
      printUsage();
      exit(-1);
    }
  }
  for ( int i=0; i<(argc-1)/2; ++i ) {
    std::string key(argv[2*i+1]), value(argv[2*i+2]);

    size_t start = key.find_first_not_of("-");
    if ( start != std::string::npos ) {
      key = key.substr(start);
    }

    if ( key.size() == 1 && expand.find(key[0]) != expand.end() ) {
      key = expand[key[0]];
    }

    if ( values.find(key) != values.end() ) {
      values[key].value = value;
    } else {
      std::cerr << "Unknown parameter: '" << key << "'" << std::endl;
      printUsage();
      exit(-1);
    }
  }
  if ( (argc & 1) == 0 ) {
    values["file"].value = std::string(argv[argc-1]);
  }
}

OptionValue::operator bool() const
{
  std::stringstream s(value);
  bool ret;
  s >> ret;
  return ret;
}

OptionValue::operator int() const
{
  std::stringstream s(value);
  int ret;
  s >> ret;
  return ret;
}

OptionValue::operator float() const
{
  std::stringstream s(value);
  float ret;
  s >> ret;
  return ret;
}

OptionValue::operator double() const
{
  std::stringstream s(value);
  double ret;
  s >> ret;
  return ret;
}

OptionValue::operator std::string() const
{
  return value;
}


std::ostream& operator<<(std::ostream& s, const OptionValue& v)
{
  s << "--" << v.name;
  if ( v.abbrev != 0 ) {
    s << " (-" << v.abbrev << ")";
  }
  s << "\t\t" << v.description << " Value is '" << v.value << "'.";
  return s;
}

