#include <string>
#include <vector>

namespace asql {

enum Type {};

class Column {
public:
  std::string name;
  std::string type;
  std::string anno;

  Column(std::string_view name, std::string_view type, std::string_view anno)
    : name(name), type(type), anno(anno) {}
};

class Table {
public:
  std::string name;
  std::vector<Column> columns;

  Table(std::string_view name, std::vector<Column> columns)
    : name(name), columns(columns) {}
};
}  // namespace asql
