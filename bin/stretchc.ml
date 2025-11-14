let () =
  let filename = Sys.argv.(1) in
  let channel = In_channel.open_text filename in
  let numbers = Stretch2.parse_stretch_source_code channel in
  List.iter (fun i -> print_int i; print_newline ()) numbers
