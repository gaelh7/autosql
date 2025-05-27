CREATE TABLE test (
  id INT32 /* NOT NULL */ UNIQUE DEFAULT (5),
  id2 INT32 UNIQUE DEFAULT (1) CHECK (5 = MAX(2 + x^2, info, 3) + - 1),
  id3 STRING DEFAULT ('abc')
);

CREATE TABLE test2 (
  id INT32 /* NOT NULL */ UNIQUE DEFAULT (5),
  id2 INT32 UNIQUE DEFAULT (1) CHECK (info + 1 = 2 + (info + 4) ^ 1),
  FOREIGN KEY (id, id2) REFERENCES test(id2, id),
  id3 STRING AS ('abc') REFERENCES test(id2),
  CHECK (id + id2 = 10)
) PRIMARY KEY (id, id2);
