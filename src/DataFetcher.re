module CustomClient = BsSocket.Client.Make(TicTacToeCommon);

let socket = CustomClient.create();

type state = {
  player: TicTacToeCommon.player,
  cell: int,
};

type action =
  | SetPlayer(TicTacToeCommon.player)
  | SetCell(int);

let component = ReasonReact.reducerComponent("DataFetcher");

let make = _children => {
  ...component,
  initialState: () => {player: Empty, cell: (-1)},
  reducer: (action, state: state) =>
    switch (action) {
    | SetPlayer(player) => ReasonReact.Update({...state, player})
    | SetCell(cell) => ReasonReact.Update({...state, cell})
    },
  didMount: self => {
    CustomClient.emit(socket, ClientConnect, ());
    CustomClient.on(socket, You, player => self.send(SetPlayer(player)));
    CustomClient.on(socket, ClientClick, ({cell}) =>
      self.send(SetCell(cell))
    );
  },
  render: self =>
    switch (self.state.player) {
    | Empty => ReasonReact.string("waiting for player")
    | player =>
      <Main
        you=player
        cell=self.state.cell
        onTurnChange=(
          cell =>
            CustomClient.emit(
              socket,
              ClientClick,
              {TicTacToeCommon.player, cell},
            )
        )
      />
    },
};
