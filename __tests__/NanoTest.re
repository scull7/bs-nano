open Jest;

let config = Nano.Connection.Config.make(
  ~url="http://root:password@localhost:5984",
  (),
);

let conn = Nano.Connection.make(config);

describe("Database Functions", () => {
  beforeAllAsync((finish) => Nano.Db.destroy(conn, "alice", (_) => finish()));

  testAsync("Create a database named `alice`", (finish) => {
    Nano.Db.create(conn, "alice", (result => {
      switch (result) {
      | `Error(e) => Nano_HttpError.getMessage(e) |> fail |> finish
      | `Ok(_) => pass |> finish
      }
    }))
  }); 
  testAsync("List all of the current databases", (finish) => {
    Nano.Db.list(conn, (result => {
      switch(result) {
      | `Error(e) => Nano_HttpError.getMessage(e) |> fail |> finish
      | `Ok((arrayOfDb, _)) => {
          Expect.expect(arrayOfDb)
          |> Expect.toContain("alice")
          |> finish;
        }
      }
    }));
  });
});
