function insert() {
  let data = getInput();
  if (!data) return;
  input.value = "";
  let element = createElement(data, root);

  let n = new Tree_Node(data, null, null, null, 0, 0, element);
  n.element.onclick = () => splay(n);
  disableInput();
  new Promise((resolve, reject) => {
    let choosing = createElement(null, root);
    choosing.children[0].classList.toggle("choosing");
    choosing.style.top = "60px";
    choosing.style.left = (60+preorder_list.indexOf(tree_root)*80)+"px";
    search(choosing, tree_root, data).then((result) => {
      root.removeChild(choosing);
      splay(result).then(() => {
        if (!result) {
          tree_root = n;
          preorder_list.splice(0, 0, n);
          resolve();
        }
        else if (result.data < data) {
          tree_root = n;
          result.parent = n;
          n.left = result;
          n.right = result.right;
          if (result.right) {
            result.right.parent = n;
            result.right = null;
          }
          preorder_list.splice(preorder_list.indexOf(result) + 1, 0, n);
          update_height(n);
          update_depth(n);
          resolve();
        }
        else if (result.data > data) {
          tree_root = n;
          result.parent = n;
          n.right = result;
          n.left = result.left;
          if (result.left) {
            result.left.parent = n;
            result.left = null;
          }
          preorder_list.splice(preorder_list.indexOf(result), 0, n);
          update_height(n);
          update_depth(n);
          resolve();
        }
        else {
          enableInput();
          reject("Insert failed");
        }
      });
    });
  }).then(() => {
    update(0);
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
      splay(result).then(() => {
        if (result && result.data == data) {
          if (!result.left) {
            tree_root = result.right;
            if (tree_root) tree_root.parent = null;
            resolve();
          }
          else if (!result.right) {
            tree_root = result.left;
            tree_root.parent = null;
            resolve();
          }
          else {
            let pos = preorder_list.indexOf(result);
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
            result.left.parent = null;
            result.right.parent = null;
            tree_root = result.right;
            result.right.depth = 0;
            preorder_list[pos].left = result.left;
            result.left.parent = preorder_list[pos];
            update_depth(result.right);
            splay(preorder_list[pos]).then(() => {
              resolve();
            });
          }
        }
        else {
          enableInput();
          reject("Search failed");
        }
      });
    });
  }).then(() => {
    update(0);
  });
}

function splay(node) {
  return new Promise((resolve) => {
    if (!node || !node.parent) resolve();
    else {
      let p = node.parent;
      let g = p.parent;
      if (g) {
        if (g.left == p) {
          if (p.left == node) {
            connect34(node, p, g, node.left, node.right, p.right, g.right, g, 1);
            connect34(node.left, node, p, node.left?node.left.left:null, node.left?node.left.right:null, node.right, p.right, p, 1);
          }
          else {
            connect34(p, node, g, p.left, node.left, node.right, g.right, g, 2);
          }
        }
        else {
          if (p.left == node) {
            connect34(g, node, p, g.left, node.left, node.right, p.right, g, 2);
          }
          else {
            connect34(g, p, node, g.left, p.left, node.left, node.right, g, 1);
            connect34(p, node, node.right, p.left, node.left, node.right?node.right.left:null, node.right?node.right.right:null, p, 1);
          }
        }
      }
      else if (p) {
        if (p.left == node) {
          connect34(node.left, node, p, node.left?node.left.left:null, node.left?node.left.right:null, node.right, p.right, p, 1);
        }
        else {
          connect34(p, node, node.right, p.left, node.left, node.right?node.right.left:null, node.right?node.right.right:null, p, 1);
        }
      }
      update(0).then(() => {
        splay(node).then(() => {
          resolve();
        })
      })
    }
  })
}