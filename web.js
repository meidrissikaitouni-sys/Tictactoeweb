const boardEl = document.getElementById("board");
const statusEl = document.getElementById("statusText");
const modeEl = document.getElementById("mode");
const difficultyEl = document.getElementById("difficulty");
const newGameBtn = document.getElementById("newGameBtn");
const resetScoreBtn = document.getElementById("resetScoreBtn");
const scoreXEl = document.getElementById("scoreX");
const scoreOEl = document.getElementById("scoreO");
const scoreDrawEl = document.getElementById("scoreDraw");

const WIN_LINES = [
  [0, 1, 2],
  [3, 4, 5],
  [6, 7, 8],
  [0, 3, 6],
  [1, 4, 7],
  [2, 5, 8],
  [0, 4, 8],
  [2, 4, 6],
];

const score = { X: 0, O: 0, draw: 0 };

const state = {
  board: Array(9).fill(""),
  current: "X",
  gameOver: false,
  winner: "",
  winningLine: null,
};

function isAiPlayer(symbol) {
  if (modeEl.value === "human-human") return false;
  if (modeEl.value === "human-ai") return symbol === "O";
  return symbol === "X";
}

function nextTurnMessage() {
  if (state.gameOver) {
    if (state.winner) return `${state.winner} gagne cette manche.`;
    return "Match nul.";
  }

  if (isAiPlayer(state.current)) {
    return `IA (${state.current}) reflechit...`;
  }

  return `A toi de jouer (${state.current}).`;
}

function evaluateBoard(board) {
  for (const line of WIN_LINES) {
    const [a, b, c] = line;
    if (board[a] && board[a] === board[b] && board[a] === board[c]) {
      return { winner: board[a], line };
    }
  }

  if (board.every((cell) => cell)) {
    return { winner: "", line: null, draw: true };
  }

  return null;
}

function minimax(board, maximizing, ai, human, depth, alpha, beta) {
  const result = evaluateBoard(board);
  if (result) {
    if (result.winner === ai) return 10 - depth;
    if (result.winner === human) return depth - 10;
    return 0;
  }

  if (maximizing) {
    let best = -Infinity;
    for (let i = 0; i < 9; i++) {
      if (board[i]) continue;
      board[i] = ai;
      const scoreValue = minimax(board, false, ai, human, depth + 1, alpha, beta);
      board[i] = "";
      best = Math.max(best, scoreValue);
      alpha = Math.max(alpha, best);
      if (beta <= alpha) break;
    }
    return best;
  }

  let best = Infinity;
  for (let i = 0; i < 9; i++) {
    if (board[i]) continue;
    board[i] = human;
    const scoreValue = minimax(board, true, ai, human, depth + 1, alpha, beta);
    board[i] = "";
    best = Math.min(best, scoreValue);
    beta = Math.min(beta, best);
    if (beta <= alpha) break;
  }
  return best;
}

function randomMove() {
  const free = state.board
    .map((value, index) => (value ? -1 : index))
    .filter((idx) => idx !== -1);
  if (!free.length) return -1;
  return free[Math.floor(Math.random() * free.length)];
}

function bestMove(ai) {
  const human = ai === "X" ? "O" : "X";
  let bestIndex = -1;
  let bestScore = -Infinity;

  for (let i = 0; i < 9; i++) {
    if (state.board[i]) continue;
    state.board[i] = ai;
    const scoreValue = minimax(state.board, false, ai, human, 0, -Infinity, Infinity);
    state.board[i] = "";

    if (scoreValue > bestScore) {
      bestScore = scoreValue;
      bestIndex = i;
    }
  }

  return bestIndex;
}

function playMove(index) {
  if (state.gameOver || state.board[index]) return;

  state.board[index] = state.current;
  const result = evaluateBoard(state.board);

  if (result) {
    state.gameOver = true;
    state.winner = result.winner;
    state.winningLine = result.line;

    if (result.winner === "X") score.X += 1;
    else if (result.winner === "O") score.O += 1;
    else score.draw += 1;
  } else {
    state.current = state.current === "X" ? "O" : "X";
  }

  render();
  maybeAiTurn();
}

function maybeAiTurn() {
  if (state.gameOver || !isAiPlayer(state.current)) return;

  setTimeout(() => {
    if (state.gameOver || !isAiPlayer(state.current)) return;

    const idx = difficultyEl.value === "hard" ? bestMove(state.current) : randomMove();
    if (idx !== -1) playMove(idx);
  }, 320);
}

function resetBoard() {
  state.board = Array(9).fill("");
  state.current = "X";
  state.gameOver = false;
  state.winner = "";
  state.winningLine = null;
  render();
  maybeAiTurn();
}

function resetScore() {
  score.X = 0;
  score.O = 0;
  score.draw = 0;
  updateScore();
}

function updateScore() {
  scoreXEl.textContent = String(score.X);
  scoreOEl.textContent = String(score.O);
  scoreDrawEl.textContent = String(score.draw);
}

function render() {
  boardEl.innerHTML = "";

  for (let i = 0; i < 9; i++) {
    const cell = document.createElement("button");
    cell.className = "cell";
    cell.setAttribute("role", "gridcell");
    cell.setAttribute("aria-label", `Case ${i + 1}`);
    cell.textContent = state.board[i];
    cell.disabled = Boolean(state.board[i]) || state.gameOver || isAiPlayer(state.current);

    if (state.winningLine && state.winningLine.includes(i)) {
      cell.classList.add("win");
    }

    cell.addEventListener("click", () => playMove(i));
    boardEl.appendChild(cell);
  }

  statusEl.textContent = nextTurnMessage();
  updateScore();
}

modeEl.addEventListener("change", resetBoard);
difficultyEl.addEventListener("change", maybeAiTurn);
newGameBtn.addEventListener("click", resetBoard);
resetScoreBtn.addEventListener("click", () => {
  resetScore();
  resetBoard();
});

resetBoard();
