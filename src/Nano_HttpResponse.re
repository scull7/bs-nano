let parser = (decoder, callback) =>
  (. err, body, header) => {
    switch (Js.Nullable.toOption(err)) {
    | Some(e) => `Error(Nano_HttpError.fromJson(e)) |> callback
    | None =>
      Js.log2("BODY: ", body);
      Js.log2("HEADER: ", header);
      let parsedHeader = Nano_HttpHeader.fromJson(header);
      let parsedBody = decoder(body);
      `Ok((parsedBody, parsedHeader)) |> callback;
    };
    ();
  };
