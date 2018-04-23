/* http://docs.couchdb.org/en/2.1.1/api/database/common.html#get--db */
module Info = {
  module Cluster = {
    type t = {
      /*
       The number of copies of every document.
       */
      replicas: int,
      /*
       The number of range partitions.
       */
      shards: int,
      /*
       The number of consistent copies of a document that need to be read
       before a successful reply.
       */
      readQuorum: int,
      /*
       The number of copies of a document that need to be written before
       a successful reply.
       */
      writeQuorum: int,
    };
    let fromJson = json =>
      Json.Decode.{
        replicas: json |> field("n", int),
        shards: json |> field("q", int),
        readQuorum: json |> field("r", int),
        writeQuorum: json |> field("w", int),
      };
  };
  module Sizes = {
    type t = {
      /*
       Size of the live data inside the database, in bytes
       */
      active: int,
      /*
       Size of the database file on disk in bytes. Views not included
       */
      file: int,
      /*
       Uncompressed size of the database contents in bytes.
       */
      uncompressed: int /* external */
    };
    let fromJson = json =>
      Json.Decode.{
        active: json |> field("active", int),
        file: json |> field("file", int),
        uncompressed: json |> field("external", int),
      };
  };
  type t = {
    name: string,
    cluster: Cluster.t,
    compactRunning: bool,
    diskFormatVersion: int,
    documentCount: int,
    deletedDocumentCount: int,
    purgeCount: int,
    updateSequence: string,
  };
  let fromJson = json =>
    Json.Decode.{
      name: json |> field("db_name", string),
      cluster: json |> field("cluster", Cluster.fromJson),
      compactRunning: json |> field("compact_running", bool),
      diskFormatVersion: json |> field("disk_format_version", int),
      documentCount: json |> field("doc_count", int),
      deletedDocumentCount: json |> field("doc_del_count", int),
      purgeCount: json |> field("purge_seq", int),
      updateSequence: json |> field("update_seq", string),
    };
};

module Decode = {
  let dbList = Json.Decode.(array(string));
  let operationStatus = Json.Decode.(field("ok", bool));
  let passThrough = x => x;
};

let create = (conn, name, cb) =>
  conn##db##create(
    name,
    Nano_HttpResponse.parser(Decode.operationStatus, cb),
  );

let destroy = (conn, name, cb) =>
  conn##db##destroy(
    name,
    Nano_HttpResponse.parser(Decode.operationStatus, cb),
  );

let get = (conn, name, cb) =>
  conn##db##get(name, Nano_HttpResponse.parser(Info.fromJson, cb));

let list = (conn, cb) =>
  conn##db##list(Nano_HttpResponse.parser(Decode.dbList, cb));
