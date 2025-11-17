let () =
  let filename = Sys.argv.(1) in
  let channel = In_channel.open_text filename in
  let ast = Stretch2.ast_of_in_channel channel in
  Stretch2.evaluate ast
