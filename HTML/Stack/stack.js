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
    duration: DURATION
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
    top: "-50px",
    left: "-50px"
  }, {
    duration: DURATION
  }).then(() => {
    root.removeChild(result);
    enableInput();
  });
  len --;
}