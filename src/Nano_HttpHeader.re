exception InvalidAccept(string);

exception InvalidCacheControl(string);

exception InvalidContentType(string);

let notString = "not_string";

module Accept = {
  type t =
    | JSON
    | All;
  let fromString =
    fun
    | "application/json" => JSON
    | "*/*" => All
    | invalid => raise(InvalidAccept(invalid));
  let fromJson = Json.Decode.(withDefault(All, map(fromString, string)));
};

module ContentType = {
  type t =
    | JSON
    | TextPlain
    | TextPlainUtf8
    | OctetStream;
  let fromString =
    fun
    | "application/json" => JSON
    | "application/octet-stream" => OctetStream
    | "text/plain" => TextPlain
    | "textplain;charset=utf-8" => TextPlainUtf8
    | invalid => raise(InvalidContentType(invalid));
  let fromJson = Json.Decode.(map(fromString, string));
};

module CacheControl = {
  type t =
    | MustRevalidate
    | Other(string);
  let fromString =
    fun
    | "must-revalidate" => MustRevalidate
    | x => Other(x);
  let fromJson = Json.Decode.(map(fromString, string));
};

type t = {
  xCouchBodyTime: option(int),
  xCouchRequestId: option(string),
  date: option(int),
  contentType: option(ContentType.t),
  cacheControl: option(CacheControl.t),
  statusCode: option(int),
  uri: option(string),
};

let fromJson = json => {
  let timestamp =
    Json.Decode.(map(int_of_float) @@ map(Js.Date.getTime) @@ date);
  Json.Decode.{
    xCouchBodyTime: json |> optional @@ field("x-couchdb-body-time", int),
    xCouchRequestId: json |> optional @@ field("x-couch-request-id", string),
    date: json |> optional @@ field("date", timestamp),
    contentType:
      json |> optional @@ field("content-type", ContentType.fromJson),
    cacheControl:
      json |> optional @@ field("cache-control", CacheControl.fromJson),
    statusCode: json |> optional @@ field("statusCode", int),
    uri: json |> optional @@ field("uri", string),
  };
};
