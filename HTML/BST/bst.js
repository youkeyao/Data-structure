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
    let current = tree_root ? (data < tree_root.data ? tree_root.left : tree_root.right) : null;
    search(choosing, current, tree_root, data).then((result) => {
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
    update(preorder_list.indexOf(n));
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
    let current = tree_root ? (data < tree_root.data ? tree_root.left : tree_root.right) : null;
    search(choosing, current, tree_root, data).then((result) => {
      root.removeChild(choosing);
      if (result && result.data == data) {
        let pos = preorder_list.indexOf(result);
        if (!remove_node(result)) {
          Velocity(preorder_list[pos].element, {
            top: preorder_list[pos+1].element.offsetTop + "px",
            left: preorder_list[pos+1].element.offsetLeft + "px"
          }, {
            duration: 500
          });
          Velocity(preorder_list[pos+1].element, {
            top: preorder_list[pos].element.offsetTop + "px",
            left: preorder_list[pos].element.offsetLeft + "px"
          }, {
            duration: 500
          }).then(() => {
            root.removeChild(preorder_list[pos+1].element);
            preorder_list.splice(pos+1, 1);
            resolve(Math.max(pos-1, 0));
          });
        }
        else {
          root.removeChild(preorder_list[pos].element);
          preorder_list.splice(pos, 1);
          resolve(Math.max(pos-1, 0));
        }
      }
      else {
        enableInput();
        reject("Search failed");
      }
    });
  }).then((pos) => {
    update(pos);
  });
}