const queue = [];
let len = 0;

function enqueue() {
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
  queue.push(element);
}

function dequeue() {
  if (!len) return;
  let result = queue.shift();
  disableInput();
  Velocity(result, {
    top: "0",
    left: "0"
  }, {
    duration: 500,
    delay: 0
  }).then(() => {
    root.removeChild(result);
    for (let i = 0; i < queue.length; i++) {
      Velocity(queue[i], {
        left: (60+i*80)+"px"
      }, {
        duration: 500,
        delay: 0
      });
    }
    setTimeout(() => {
      enableInput();
    }, 500);
  });
  len --;
}