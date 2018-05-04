type player =
  | X
  | O
  | Empty;

type clientClickData = {
  player,
  cell: int,
};

type t('a) =
  | ClientConnect: t('a)
  | ClientClick: t(clientClickData)
  | You: t(player);

let stringify = (type a, t: t(a)) =>
  switch (t) {
  | ClientConnect => "ClientConnect"
  | ClientClick => "ClientClick"
  | You => "You"
  };
