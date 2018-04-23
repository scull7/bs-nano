module Config = Nano_Config;

type callback = (. Js.Nullable.t(Js.Json.t), Js.Json.t, Js.Json.t) => unit;

type t = {
  .
  "db": {
    .
    [@bs.meth] "create": (string, callback) => unit,
    [@bs.meth] "destroy": (string, callback) => unit,
    [@bs.meth] "get": (string, callback) => unit,
    [@bs.meth] "list": callback => unit,
  },
};

/*
  Using `Scope` here because it is listed as an alias of `nano.use`.  So we
  are assuming the `Scope` nomenclature is synonymous with `Document Store`.
 */
[@bs.send] external use : (t, string) => Nano_Scope.t = "";

/*
   Create a nano connection using the given configuration object.
 */
[@bs.module] external make : Nano_Config.t => t = "nano";
