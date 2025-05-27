CREATE TABLE test (
  id INT32[] DEFAULT (1 + (2 * 3) + 4),
  ignored INT32
);

CREATE TABLE test2 (
  id INT32 DEFAULT (0),
  ignored INT32
);
