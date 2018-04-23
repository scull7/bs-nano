/*
   An HTTP(s) agent that will keep the connection alive.
   @see https://www.npmjs.com/package/agentkeepalive
 */
module Agent = {
  module Config = {
    type t = {
      .
      "maxSockets": int,
      "maxKeepAliveRequests": int,
      "maxKeepAliveTime": int,
    };
  };
  type t;
  [@bs.new] [@bs.module] external agent : Config.t => t = "agentkeepalive";
  /*
     Create a new keepalive connection agent.
   */
  let make = (~maxSockets, ~maxKeepAliveRequests, ~maxKeepAliveTime) =>
    agent({
      "maxSockets": maxSockets,
      "maxKeepAliveRequests": maxKeepAliveRequests,
      "maxKeepAliveTime": maxKeepAliveTime,
    });
};

/*
  Nano uses the `request` module underneath.  This object represents the
  default configuration which will be passed to the `request` module during
  construction.
  @see https://www.npmjs.com/package/request
  @TODO - implement the rest of the `request module settings
 */
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

/*
   CouchDB Connection object to pass to nano.
 */
type t = {
  .
  "url": string,
  "parseUrl": bool,
  "requestDefaults": Request.t,
};

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
