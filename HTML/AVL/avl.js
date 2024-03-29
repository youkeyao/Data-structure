function insert() {
  let data = getInput();
  if (!data) return;
  input.value = "";
  let element = createElement(data, root);

  let n = new Tree_Node(data, null, null, null, 0, 0, element);
  disableInput();
  new Promise((resolve, reject) => {
    let choosing = createElement(null, root);
    choosing.children[0].classList.toggle("choosing");
    choosing.style.top = "60px";
    choosing.style.left = (60+preorder_list.indexOf(tree_root)*80)+"px";
    search(choosing, tree_root, data).then((result) => {
      root.removeChild(choosing);
      if (insert_node(result, n)) {
        resolve();
      }
      else {
        enableInput();
        reject("Insert failed");
      }
    });
  }).then(() => {
    update(preorder_list.indexOf(n)).then(() => {
      check_AVL(n);
    });;
  });
}

function remove() {
  let data = getInput();
  if (!data) return;
  input.value = "";

  disableInput();

  new Promise((resolve, reject) => {
    let choosing = createElement(null, root);
    choosing.children[0].classList.toggle("choosing");
    choosing.style.top = "60px";
    choosing.style.left = (60+preorder_list.indexOf(tree_root)*80)+"px";
    search(choosing, tree_root, data).then((result) => {
      root.removeChild(choosing);
      if (result && result.data == data) {
        let pos = preorder_list.indexOf(result);
        if (!remove_node(result)) {
          Velocity(preorder_list[pos].element, {
            top: preorder_list[pos+1].element.offsetTop + "px",
            left: preorder_list[pos+1].element.offsetLeft + "px"
          }, {
            duration: DURATION
          });
          Velocity(preorder_list[pos+1].element, {
            top: preorder_list[pos].element.offsetTop + "px",
            left: preorder_list[pos].element.offsetLeft + "px"
          }, {
            duration: DURATION
          }).then(() => {
            let element = preorder_list[pos+1].element;
            preorder_list.splice(pos+1, 1);
            Velocity(element, {
              top: "-50px",
              left: "-50px"
            }, {
              duration: DURATION
            }).then(() => {
              root.removeChild(element);
            });
            resolve(Math.min(pos+1, preorder_list.length - 1));
          });
        }
        else {
          let element = preorder_list[pos].element;
          preorder_list.splice(pos, 1);
          Velocity(element, {
            top: "-50px",
            left: "-50px"
          }, {
            duration: DURATION
          }).then(() => {
            root.removeChild(element);
          });
          resolve(Math.min(pos, preorder_list.length - 1));
        }
      }
      else {
        enableInput();
        reject("Search failed");
      }
    });
  }).then((pos) => {
    update(pos).then(() => {
      check_AVL(preorder_list[pos]);
    });
  });
}

async function check_AVL(node) {
  while (node) {
    let flag = false;
    let p = node.parent;
    let left_height = node.left ? node.left.height : -1;
    let right_height = node.right ? node.right.height : -1;
    node.height = Math.max(left_height, right_height) + 1;

    if (left_height - right_height > 1) {
      flag = true;
      let f = node.left;
      let l_height = f.left ? f.left.height : -1;
      let r_height = f.right ? f.right.height : -1;
      if (l_height > r_height) {
        connect34(f.left, f, node, f.left.left, f.left.right, f.right, node.right, node, 1);
      }
      else {
        connect34(f, f.right, node, f.left, f.right.left, f.right.right, node.right, node, 2);
      }
    }
    else if (right_height - left_height > 1) {
      flag = true;
      let f = node.right;
      let l_height = f.left ? f.left.height : -1;
      let r_height = f.right ? f.right.height : -1;
      if (l_height > r_height) {
        connect34(node, f.left, f, node.left, f.left.left, f.left.right, f.right, node, 2);
      }
      else {
        connect34(node, f, f.right, node.left, f.left, f.right.left, f.right.right, node, 1);
      }
    }
    
    if (flag) {
      await update(0);
    }

    node = p;
  }
}