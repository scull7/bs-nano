exception InvalidMethod(string);

module Method = {
  type t =
    | Delete
    | Get
    | Post
    | Put;
  let fromString = str =>
    switch (str |> String.lowercase) {
      | "delete" => Delete
      | "get" => Get
      | "post" => Post
      | "put" => Put
      | invalid => raise(InvalidMethod(invalid))
      };

  let fromJson = Json.Decode.map(fromString, Json.Decode.string);
};
