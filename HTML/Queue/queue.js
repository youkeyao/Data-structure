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
    duration: DURATION
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
    top: "-50px",
    left: "-50px"
  }, {
    duration: DURATION
  }).then(() => {
    root.removeChild(result);
    for (let i = 0; i < queue.length; i++) {
      Velocity(queue[i], {
        left: (60+i*80)+"px"
      }, {
        duration: DURATION
      });
    }
    setTimeout(() => {
      enableInput();
    }, DURATION);
  });
  len --;
}