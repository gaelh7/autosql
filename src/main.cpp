#include <filesystem>
#include <iostream>
#include <ostream>

#include "autosql/parse/database.hpp"
#include "autosql/parse/table.hpp"
#include "autosql/schema/database.hpp"

int main() {
  asql::parse::DatabaseParse db1{"/home/gaelh/projects/autosql/test/before.sql"};
  asql::parse::DatabaseParse db2{"/home/gaelh/projects/autosql/test/after.sql"};
  asql::parse::TableDiff diff{db1.tables_["test"], db2.tables_["test"]};
  asql::parse::TableDiff diff2{db1.tables_["test2"], db2.tables_["test2"]};
  std::cout << diff.sql() << std::endl;
  std::cout << diff2.sql() << std::endl;
  asql::Database database1{db1};
  asql::Database database2{db2};
  return 0;
}
