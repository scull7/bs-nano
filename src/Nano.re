module Connection = {
  module Config = {
    module Agent = {
      type t;
      type config = {
        .
        "maxSockets": int,
        "maxKeepAliveRequests": int,
        "maxKeepAliveTime": int,
      };
      [@bs.new] [@bs.module] external agent : config => t = "agentkeepalive";
      let make = (~maxSockets, ~maxKeepAliveRequests, ~maxKeepAliveTime) => {
        let config = {
          "maxSockets": maxSockets,
          "maxKeepAliveRequests": maxKeepAliveRequests,
          "maxKeepAliveTime": maxKeepAliveTime,
        };
        agent(config);
      };
    };
    /* @TODO - implement the rest of the `request` module settings */
    module Request = {
      type t = {
        .
        "agent": Agent.t,
        "proxy": Js.Nullable.t(string),
      };
      let make = (~agent, ~proxy=?, _) => {
        "agent": agent,
        "proxy": Js.Nullable.fromOption(proxy),
      };
    };
    type t = {
      .
      "url": string,
      "parseUrl": bool,
      "requestDefaults": Request.t,
    };
    /* @TODO - implement a setting for the log functions. */
    let make =
        (
          ~url,
          ~parseUrl=true,
          ~maxSockets=50,
          ~maxKeepAliveRequests=0,
          ~maxKeepAliveTime=30000,
          ~proxy=?,
          _,
        ) => {
      let agent =
        Agent.make(~maxSockets, ~maxKeepAliveRequests, ~maxKeepAliveTime);
      {
        "url": url,
        "parseUrl": parseUrl,
        "requestDefaults": Request.make(~agent, ~proxy?, ()),
      };
    };
  };
  type callback = (. Js.Nullable.t(Js.Json.t), Js.Json.t, Js.Json.t) => unit;
  type t = {
    .
    "db": {
      .
      [@bs.meth] "create": (string, callback) => unit,
      [@bs.meth] "destroy": (string, callback) => unit,
      [@bs.meth] "list": callback => unit,
    },
  };
  [@bs.module] external make : Config.t => t = "nano";
};

[@bs.send] external use : (Connection.t, string) => Connection.t = "";

let handler = (decoder, cb) => (. err, body, header) => {
  switch (Js.Nullable.toOption(err)) {
  | Some(e) => `Error(Nano_HttpError.fromJson(e)) |> cb
  | None => {
      Js.log2("BODY: ", body);
      Js.log2("HEADER: ", header);

      let parsedHeader = Nano_HttpHeader.fromJson(header);
      let parsedBody = decoder(body);
      `Ok((parsedBody, parsedHeader)) |> cb;
    }
  };
  ();
};

module Db = {
  let operationStatusDecoder = Json.Decode.(field("ok", bool));
  let passThroughDecoder = (x) => x;
  
  let create = (conn, name, cb) =>
    conn##db##create(name, handler(operationStatusDecoder, cb));
  let destroy = (conn, name, cb) =>
    conn##db##destroy(name, handler(operationStatusDecoder, cb));
  let list = (conn, cb) =>
    conn##db##list(handler(Json.Decode.(array(string)), cb));
};
