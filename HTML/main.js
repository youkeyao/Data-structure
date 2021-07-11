let root;
let input;
let select;
let op_but;
let canvas;
let context;

window.onload = () => {
  root = document.querySelector(".down");
  input = document.getElementById("input");
  select = document.getElementById("select");
  op_but = document.querySelectorAll(".up button");
  canvas = document.querySelector(".draw");
  context = canvas.getContext("2d");
};

function createElement(data, root) {
  let div = document.createElement("div");
  div.className = "element";
  let item = document.createElement("div");
  item.className = "item";
  item.innerHTML = data;
  div.appendChild(item);
  root.appendChild(div);
  return div;
}

function getInput() {
  let data = input.value;
  if (select.value == "Number") {
    return Number(data);
  }
  else {
    return data;
  }
}

function disableInput() {
  input.disabled = true;
  for (let i of op_but) {
    i.disabled = true;
  }
}

function enableInput() {
  input.disabled = false;
  for (let i of op_but) {
    i.disabled = false;
  }
}