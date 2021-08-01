let btree_root = null;
let bottom_list = [];
let m = 3;

function BTree_Node(parent, key, child, depth, items) {
  this.parent = parent;
  this.key = key;
  this.child = child;
  this.depth = depth;
  this.element = createContainer(root);
  this.items = items;
}

function update_BNode(node, pos) {
  let width = 60 * node.key.length;
  node.left = pos - width / 2;
  node.right = pos + width / 2;
  Velocity(node.element, {
    top: (80+node.depth*120)+"px",
    left: pos+"px",
    width: width+"px"
  }, {
    duration: DURATION
  });
  for (let i = 0; i < node.items.length; i ++) {
    Velocity(node.items[i], {
      top: (80+node.depth*120)+"px",
      left: node.left+(30+i*60)+"px"
    }, {
      duration: DURATION
    });
  }
}

function update_BTree() {
  return new Promise((resolve) => {
    let upNode = [];
    let count = 40;
    for (let i of bottom_list) {
      let w = 60 * i.key.length;
      update_BNode(i, count + w/2);
      count += (w + 40);
      if (i.parent && upNode[upNode.length-1] != i.parent) {
        upNode.push(i.parent);
      }
    }
    while (upNode.length != 0) {
      let i = upNode.shift();
      update_BNode(i, (i.child[0].left+i.child[i.child.length-1].right)/2);
      if (i.parent && upNode[upNode.length-1] != i.parent) {
        upNode.push(i.parent);
      }
    }
    let id = setInterval(() => {
      canvas.width = root.scrollWidth;
      canvas.height = root.scrollHeight;
      if (btree_root) {
        context.moveTo(btree_root.element.offsetLeft, 0);
        context.lineTo(btree_root.element.offsetLeft, btree_root.element.offsetTop);
        context.stroke();
        type_select.disabled = true;
        order_input.disabled = true;
      }
      else {
        type_select.disabled = false;
        order_input.disabled = false;
      }
      let queue = [];
      if (btree_root) queue.push(btree_root);
      while (queue.length != 0) {
        let e = queue.shift();
        for (let i = 0; i < e.child.length; i ++) {
          queue.push(e.child[i]);
          context.moveTo(e.element.offsetLeft - e.element.offsetWidth/2 + i*(e.element.offsetWidth/e.key.length), e.element.offsetTop + 30);
          context.lineTo(e.child[i].element.offsetLeft, e.child[i].element.offsetTop - 30);
          context.stroke();
        }
      }
    }, 10);
    setTimeout(() => {
      clearInterval(id);
      enableInput();
      resolve();
    }, DURATION + INTERVAL);
  });
}

function update_BTree_depth(node) {
  let queue = [node];
  while(queue.length != 0) {
    let e = queue.shift();
    for (let i of e.child) {
      i.depth = e.depth + 1;
      queue.push(i);
    }
  }
}

function search_BNode(choosing, node, pos, data) {
  return new Promise((resolve) => {
    if (!node) {
      resolve(null);
    }
    else {
      Velocity(choosing, {
        top: node.element.offsetTop+"px",
        left: node.items[pos].offsetLeft + "px"
      }, {
        duration: DURATION
      }).then(() => {
        if (data < node.key[pos]) {
          if (node.child[pos]) {
            search_BNode(choosing, node.child[pos], 0, data).then((result) => {
              resolve(result);
            });
          }
          else {
            setTimeout(() => {
              resolve([node, pos]);
            }, DURATION);
          }
        }
        else if (data == node.key[pos]) {
          setTimeout(() => {
            resolve([node, pos]);
          }, DURATION);
        }
        else {
          if (pos == node.key.length - 1) {
            if (node.child[pos+1]) {
              search_BNode(choosing, node.child[pos+1], 0, data).then((result) => {
                resolve(result);
              });
            }
            else {
              setTimeout(() => {
                resolve([node, pos+1]);
              }, DURATION);
            }
          }
          else {
            search_BNode(choosing, node, pos+1, data).then((result) => {
              resolve(result);
            });
          }
        }
      });
    }
  });
}

function insert() {
  let data = getInput();
  if (!data || Number(order_input.value) < 3) return;
  m = parseInt(order_input.value);
  order_input.value = m;
  input.value = "";
  let element = createElement(data, root);

  disableInput();
  new Promise((resolve, reject) => {
    let choosing = createElement(null, root);
    choosing.children[0].classList.toggle("choosing");
    if (btree_root) {
      choosing.style.left = btree_root.items[0].offsetLeft + "px";
      choosing.style.top = btree_root.element.offsetTop + "px";
    }
    search_BNode(choosing, btree_root, 0, data).then((result) => {
      root.removeChild(choosing);
      if (!result) {
        btree_root = new BTree_Node(null, [data], [], 0, [element])
        bottom_list.push(btree_root);
        btree_root.element.style.left = "-50px";
        btree_root.element.style.top = "-50px";
        resolve(btree_root);
      }
      else if (!(result[0].key.includes(data))) {
        result[0].key.splice(result[1], 0, data);
        result[0].items.splice(result[1], 0, element);
        resolve(result[0]);
      }
      else {
        enableInput();
        reject("Insert failed");
      }
    });
  }).then((node) => {
    update_BTree().then(() => {
      check_overflow(node);
    });
  });
}

function remove() {
  let data = getInput();
  if (!data || Number(order_input.value) < 3) return;
  m = parseInt(order_input.value);
  order_input.value = m;
  input.value = "";

  disableInput();

  new Promise((resolve, reject) => {
    let choosing = createElement(null, root);
    choosing.children[0].classList.toggle("choosing");
    if (btree_root) {
      choosing.style.left = btree_root.items[0].offsetLeft + "px";
      choosing.style.top = btree_root.element.offsetTop + "px";
    }
    search_BNode(choosing, btree_root, 0, data).then((result) => {
      root.removeChild(choosing);
      if (result && result[0].key[result[1]] == data) {
        if (result[0].child[0]) {
          let old = result[0];
          let old_pos = result[1];
          let old_item = result[0].items[result[1]];
          result[0] = result[0].child[result[1] + 1];
          result[1] = 0;
          while (result[0].child[0]) {
            result[0] = result[0].child[0];
            result[1] = 0;
          }
          old.key[old_pos] = result[0].key[result[1]];
          result[0].key[result[1]] = data;
          old.items[old_pos] = result[0].items[result[1]];
          result[0].items[result[1]] = old_item;
          Velocity(old.items[old_pos], {
            top: old_item.offsetTop + "px",
            left: old_item.offsetLeft + "px"
          }, {
            duration: DURATION
          });
          Velocity(old_item, {
            top: old.items[old_pos].offsetTop + "px",
            left: old.items[old_pos].offsetLeft + "px"
          }, {
            duration: DURATION
          }).then(() => {
            let element = result[0].items[result[1]];
            result[0].key.splice(result[1], 1);
            result[0].items.splice(result[1], 1);
            Velocity(element, {
              left: "-50px",
              top: "-50px"
            }, {
              duration: DURATION
            }).then(() => {
              root.removeChild(element);
            });
            resolve(result[0]);
          });
        }
        else {
          let element = result[0].items[result[1]];
          result[0].key.splice(result[1], 1);
          result[0].items.splice(result[1], 1);
          Velocity(element, {
            left: "-50px",
            top: "-50px"
          }, {
            duration: DURATION
          }).then(() => {
            root.removeChild(element);
          });
          resolve(result[0]);
        }
      }
      else {
        enableInput();
        reject("Search failed");
      }
    });
  }).then((node) => {
    update_BTree().then(() => {
      check_underflow(node);
    })
  });
}

async function check_overflow(node) {
  if (!node || node.key.length < m) return;
  let mid = parseInt(m / 2);
  let p = node.parent;
  if (!p) {
    p = btree_root = new BTree_Node(null, [], [], 0, []);
    p.element.style.left = node.element.offsetLeft + "px";
    p.element.style.top = node.element.offsetTop + "px";
    p.child.splice(0, 0, node);
    node.parent = p;
  }
  let new_node = new BTree_Node(p, [], [], node.depth, []);
  new_node.element.style.left = node.element.offsetLeft + "px";
  new_node.element.style.top = node.element.offsetTop + "px";
  new_node.element.style.width = node.element.offsetWidth + "px";
  let flag = bottom_list.indexOf(node);
  if (flag >= 0) {
    bottom_list.splice(flag, 0, new_node);
  }
  let pos = p.child.indexOf(node);
  p.key.splice(pos, 0, node.key[mid]);
  p.child.splice(pos, 0, new_node);
  p.items.splice(pos, 0, node.items[mid]);
  while (mid --) {
    new_node.key.push(node.key.shift());
    if (flag == -1) {
      new_node.child.push(node.child.shift());
      new_node.child[new_node.child.length-1].parent = new_node;
    }
    new_node.items.push(node.items.shift());
  }
  node.key.shift();
  node.items.shift();
  if (flag == -1) {
    new_node.child.push(node.child.shift());
    new_node.child[new_node.child.length-1].parent = new_node;
  }
  if (node.depth == 0) update_BTree_depth(p);
  await update_BTree();
  check_overflow(p);
}

async function check_underflow(node) {
  if (!node || node.key.length >= (m-1)/2) return;
  let p = node.parent;
  if (!p) {
    if (node.key.length == 0) {
      if (node.child[0]) {
        btree_root = node.child[0];
        btree_root.parent = null;
        btree_root.depth = 0;
        root.removeChild(node.element);
        update_BTree_depth(btree_root);
      }
      else {
        btree_root = null;
        bottom_list.splice(0, 1);
        root.removeChild(node.element);
      }
    }
  }
  else {
    let pos = p.child.indexOf(node);
    if (pos != 0 && p.child[pos-1].key.length > (m-1)/2) {
      let l = p.child[pos-1];
      node.key.splice(0, 0, p.key[pos-1]);
      node.items.splice(0, 0, p.items[pos-1]);
      p.key[pos-1] = l.key[l.key.length - 1];
      p.items[pos-1] = l.items[l.items.length - 1];
      l.key.pop();
      l.items.pop();
      if (l.child[0]) {
        node.child.splice(0, 0, l.child.pop());
        node.child[0].parent = node;
      }
    }
    else if (pos != p.child.length-1 && p.child[pos+1].key.length > (m-1)/2) {
      let r = p.child[pos+1];
      node.key.push(p.key[pos]);
      node.items.push(p.items[pos]);
      p.key[pos] = r.key[0];
      p.items[pos] = r.items[0];
      r.key.shift();
      r.items.shift();
      if (r.child[0]) {
        node.child.push(r.child.shift());
        node.child[node.child.length-1].parent = node;
      }
    }
    else {
      let flag = bottom_list.indexOf(node);
      if (pos > 0) {
        let l = p.child[pos-1];
        l.key.push(p.key.splice(pos-1, 1)[0]);
        l.items.push(p.items.splice(pos-1, 1)[0]);
        if (flag == -1) {
          l.child.push(node.child.shift());
          l.child[l.child.length-1].parent = l;
        }
        p.child.splice(pos, 1);
        while (node.key.length) {
          l.key.push(node.key.shift());
          if (flag == -1) {
            l.child.push(node.child.shift());
            l.child[l.child.length-1].parent = l;
          }
          l.items.push(node.items.shift());
        }
        if (flag != -1) bottom_list.splice(flag, 1);
        root.removeChild(node.element);
      }
      else {
        let r = p.child[pos+1];
        node.key.push(p.key.splice(pos, 1)[0]);
        node.items.push(p.items.splice(pos, 1)[0]);
        if (flag == -1) {
          node.child.push(r.child.shift());
          node.child[node.child.length-1].parent = node;
        }
        p.child.splice(pos+1, 1);
        while (r.key.length) {
          node.key.push(r.key.shift());
          if (flag == -1) {
            node.child.push(r.child.shift());
            node.child[node.child.length-1].parent = node;
          }
          node.items.push(r.items.shift());
        }
        if (flag != -1) bottom_list.splice(flag+1, 1);
        root.removeChild(r.element);
      }
    }
  }
  await update_BTree();
  check_underflow(p);
}