(* Umesh Dhakal *)
(* Mean, Median and Mode in OCaml *)


let split_by_comma (line_text : string) : string list =
  line_text
  |> String.split_on_char ','
  |> List.map String.trim
  |> List.filter (fun s -> s <> "")

(* changeing string list to int list *)
let to_int_list (parts : string list) : int list =
  parts
  |> List.map (fun s ->
       try int_of_string s
       with _ -> failwith ("Invalid integer: " ^ s))

(* reading a input integers *)
let read_numbers () : int list =
  let args_list = Array.to_list Sys.argv in
  match args_list with
  | _ :: args when args <> [] ->
      let combined = String.concat " " args |> String.trim in
      if String.contains combined ',' then
        combined |> split_by_comma |> to_int_list
      else
        args |> to_int_list
  | _ ->
      let user_line = read_line () |> String.trim in
      if user_line = "" then failwith "Please enter the number, no number enter.";
      user_line |> split_by_comma |> to_int_list

(* mean = sum / total number*)
let get_mean (value_list : int list) : float =
  let total_sum = List.fold_left (fun acc x -> acc + x) 0 value_list in
  (float_of_int total_sum) /. (float_of_int (List.length value_list))

(* median *)
let get_median (value_list : int list) : float =
  let sorted_vals = List.sort compare value_list in
  let total_count = List.length sorted_vals in
  let mid = total_count / 2 in
  if total_count mod 2 = 1 then
    float_of_int (List.nth sorted_vals mid)
  else
    let left_val = List.nth sorted_vals (mid - 1) in
    let right_val = List.nth sorted_vals mid in
    ((float_of_int left_val) +. (float_of_int right_val)) /. 2.0

(* mode *)
let get_mode (value_list : int list) : int list option =
  let sorted_vals = List.sort compare value_list in

  let rec group_count current_val current_count result_list rest =
    match rest with
    | [] -> List.rev ((current_val, current_count) :: result_list)
    | x :: xs ->
        if x = current_val then
          group_count current_val (current_count + 1) result_list xs
        else
          group_count x 1 ((current_val, current_count) :: result_list) xs
  in

  let grouped =
    match sorted_vals with
    | [] -> []
    | x :: xs -> group_count x 1 [] xs
  in

  (* find max frequency *)
  let max_freq =
    List.fold_left (fun best (_, c) -> max best c) 1 grouped
  in

  (* if all are once there is no mode *)
  if max_freq = 1 then None
  else
    let mode_list =
      grouped
      |> List.filter (fun (_, c) -> c = max_freq)
      |> List.map fst
    in
    Some mode_list

let () =
  try
    print_string "Enter all the integers number separated by commas: ";
    let numbers_entered = read_numbers () in

    let total_count = List.length numbers_entered in
    let mean_result = get_mean numbers_entered in
    let median_result = get_median numbers_entered in
    let mode_result = get_mode numbers_entered in

    Printf.printf "Total count: %d\n" total_count;
    Printf.printf "The mean of all the integers: %.4f\n" mean_result;
    Printf.printf "The median of all the integers: %.4f\n" median_result;

    (match mode_result with
     | None ->
         Printf.printf "There is no mode as all the number repeat same amount of time\n"
     | Some modes ->
         let mode_text = String.concat ", " (List.map string_of_int modes) in
         Printf.printf "The mode of all the integers: %s\n" mode_text)

  with
  | Failure msg ->
      Printf.eprintf "Wrong Input: %s\n" msg;
      exit 1
  | _ ->
      Printf.eprintf "Error,Something went wrong.\n";
      exit 1
