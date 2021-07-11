const stack = [];
let len = 0;

function push() {
  let data = getInput();
  if (!data) return;
  input.value = "";
  let element = createElement(data, root);
  disableInput();
  Velocity(element, {
    top: "50%",
    left: (60+len*80)+"px"
  }, {
    duration: 500,
    delay: 0
  }).then(() => {
    enableInput();
  });
  len++;
  stack.push(element);
}

function pop() {
  if (!len) return;
  let result = stack.pop();
  disableInput();
  Velocity(result, {
    top: "0",
    left: "0"
  }, {
    duration: 500,
    delay: 0
  }).then(() => {
    root.removeChild(result);
  }).then(() => {
    enableInput();
  });
  len --;
}