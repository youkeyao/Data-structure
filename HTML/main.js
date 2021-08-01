const DURATION = 500;
const INTERVAL = 0;
let root;
let input;
let type_select;
let order_input;
let op_but;
let canvas;
let context;

window.onload = () => {
  root = document.querySelector(".down");
  input = document.getElementById("input");
  type_select = document.getElementById("select");
  order_input = document.getElementById("order");
  op_but = document.querySelectorAll(".up button");
  canvas = document.querySelector(".draw");
  context = canvas.getContext("2d");
};

function createElement(data, parent) {
  let div = document.createElement("div");
  div.className = "element";
  let item = document.createElement("div");
  item.className = "item";
  item.innerHTML = data;
  item.style.transition = "background-color " + (DURATION / 1000) + "s";
  div.appendChild(item);
  parent.appendChild(div);
  return div;
}

function createContainer(parent) {
  let div = document.createElement("div");
  div.className = "container";
  parent.appendChild(div);
  return div;
}

function getInput() {
  let data = input.value;
  if (type_select.value == "Number") {
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