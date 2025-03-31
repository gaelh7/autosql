#pragma once

#include <string>
#include <vector>
#include "autosql/constraint.h"

namespace asql {
enum Type {};

enum Storage { PLAIN, EXTERNAL, EXTENDED, MAIN, DEFAULT };

class Column {
public:
  std::string name;
  std::string type;
  std::string storage;
  std::string compression;
  std::string collate;
  std::vector<Constraint> constraints;

  Column() = default;

  Column(std::string_view name, std::string_view type)
    : name(name), type(type) {}
};
}  // namespace asql
