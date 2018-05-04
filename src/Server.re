module InnerServer = BsSocket.Server.Make(TicTacToeCommon);

let playerX = ref(None);

let playerO = ref(None);

let startSocketIOServer = http => {
  let io = InnerServer.createWithHttp(http);
  InnerServer.onConnect(
    io,
    socket => {
      open InnerServer;
      print_endline("got connection");
      Socket.on(socket, ClientConnect, (_) =>
        switch (playerX^) {
        | None => playerX := Some(socket)
        | Some(playerXSocket) =>
          switch (playerO^) {
          | None =>
            playerO := Some(socket);
            Socket.emit(
              playerXSocket,
              TicTacToeCommon.You,
              TicTacToeCommon.X,
            );
            Socket.emit(socket, TicTacToeCommon.You, TicTacToeCommon.O);
          | Some(_) => ()
          }
        }
      );
      Socket.on(
        socket,
        ClientClick,
        ({TicTacToeCommon.player, cell} as clientClickData) => {
          print_endline(
            "ClientClick: "
            ++ (
              switch (player) {
              | X => "X"
              | O => "O"
              | Empty => "Empty"
              }
            )
            ++ " "
            ++ string_of_int(cell),
          );
          let otherPlayerSocket =
            switch (player) {
            | X => playerO^
            | O => playerX^
            | Empty => failwith("player missing!")
            };
          let otherPlayerSocket =
            switch (otherPlayerSocket) {
            | Some(otherPlayerSocket) => otherPlayerSocket
            | None => failwith("other player socket missing!")
            };
          Socket.emit(otherPlayerSocket, ClientClick, clientClickData);
        },
      );
    },
  );
  print_endline("starting socket server");
};
