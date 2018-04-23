/*
  Using `Scope` here because it is listed as an alias of `nano.use`.  So we
  are assuming the `Scope` nomenclature is synonymous with `Document Store`.
 */
type callback = (. Js.Nullable.t(Js.Json.t), Js.Json.t, Js.Json.t) => unit;

type doc = Js.Json.t;

type docname = string;

type indexDefinition;

type keys;

type params;

type copyOptions;

type fetchOptions;

type listOptions;

type rev = string;

type t;

/*
   Inserts `doc` into the data base with optional `params`. If `docname`
   is NULL then an `_id` will be automatically generated.
 */
[@bs.send]
external create : (doc, Js.Nullable.t(docname), callback) => unit = "insert";

/*
   Removes a document from CouchDB whose `_id` is `docname` and who's
   revision is `_rev`.
 */
[@bs.send] external destroy : (docname, rev, callback) => unit = "";

/*
   Get a single document from CouchDB by `_id` (`docname`).
 */
[@bs.send] external get : (docname, params, callback) => unit = "";

/*
   Same as `get`, but a lightweight version that only returns the headers.
 */
[@bs.send] external head : (docname, callback) => unit = "";

/*
   Copies the contents (and attachments) of a document to a new document,
   or overwrite an existing target document.
 */
[@bs.send]
external copy : (docname, docname, copyOptions, callback) => unit = "";

/*
   Bulk operations (update/delete/insert) on the database.
   @TODO - work out the API to this functionality to be easier to use.
 */
[@bs.send] external bulk : (Js.Json.t, callback) => unit = "";

/*
   List all the documents in the database.
 */
[@bs.send] external list_ : (listOptions, callback) => unit = "";

/*
   Bulk fetch of the database documents by a list of docnames. `include_docs`
   is always set to true.
 */
[@bs.send] external fetch : (keys, fetchOptions, callback) => unit = "";

/*
   Bulk fetch of the revisions of the datrabase documents. Same method as
   `fetch` but `include_docs` is not automatically set to `true`.
 */
[@bs.send] external fetRevs : (keys, fetchOptions, callback) => unit = "";

/*
   Create an index on database fields.
 */
[@bs.send] external createIndex : (indexDefinition, callback) => unit = "";
