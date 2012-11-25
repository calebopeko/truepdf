// -*- c++ -*-
#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
#include <ostream>
#include <map>

class OptionValue
{
public:

  OptionValue() {}

  OptionValue(const std::string& name_, const std::string& initial_, const std::string& description_, const char abbrev_=0)
    : value(initial_), name(name_), description(description_), abbrev(abbrev_) {}

  operator bool() const;
  operator int() const;
  operator float() const;
  operator double() const;
  operator std::string() const;

  friend std::ostream& operator<<(std::ostream& s, const OptionValue& v);

  friend class Options;

private:

  std::string value, name, description;
  
  char abbrev;
};

std::ostream& operator<<(std::ostream& s, const OptionValue& v);

class Options
{
public:

  Options();

  Options(int argc, char** argv);

  void init();

  void parse(int argc, char** argv);

  OptionValue& operator[](const std::string k) { return values[k]; }

  void printUsage();

  void dump(const std::string& filename);

private:

  void addParam(const std::string& name, const std::string& initial, const std::string& description, char abbrev=0);

  void addParam(const OptionValue& v);

  std::map<std::string, OptionValue> values;

  std::map<char, std::string> expand;

};

#endif
