module Request = {
  module Headers = {
    type t = {
      contentType: option(Nano_HttpHeader.ContentType.t),
      accept: option(Nano_HttpHeader.Accept.t),
    };
    let fromJson = json =>
      Json.Decode.{
        contentType:
          json
          |> optional(
               field("content-type", Nano_HttpHeader.ContentType.fromJson),
             ),
        accept:
          json |> optional(field("accept", Nano_HttpHeader.Accept.fromJson)),
      };
  };
  type t = {
    method: option(Nano_Http.Method.t),
    headers: option(Headers.t),
    uri: option(string),
  };
  let fromJson = json =>
    Json.Decode.{
      method: json |> optional(field("method", Nano_Http.Method.fromJson)),
      headers: json |> optional(field("headers", Headers.fromJson)),
      uri: json |> optional(field("uri", string)),
    };
};

type t = {
  name: string,
  error: string,
  reason: string,
  scope: string,
  statusCode: int,
  request: Request.t,
  headers: Nano_HttpHeader.t,
  errid: string,
  description: string,
};

let fromJson = json =>
  Json.Decode.{
    name: json |> field("name", string),
    error: json |> field("error", string),
    reason: json |> field("reason", string),
    scope: json |> field("scope", string),
    statusCode: json |> field("statusCode", int),
    request: json |> field("request", Request.fromJson),
    headers: json |> field("headers", Nano_HttpHeader.fromJson),
    errid: json |> field("errid", string),
    description: json |> field("description", string),
  };

let getMessage = e => {
  let {name, reason, description} = e;
  {j|$name - $reason - $description|j};
};
