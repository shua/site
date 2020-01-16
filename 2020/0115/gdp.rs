mod named {
    use core::marker::PhantomData;

    #[derive(Clone)]
    pub struct Named<Name, A>(pub A, PhantomData<Name>);

    // don't use this, use the macro name!
    pub fn name<Name: Fn(), T, A, K: Fn(Named<Name, A>) -> T>(_: Name, x: A, k: K) -> T {
        k(Named(x, PhantomData))
    }
    
    #[macro_export]
    macro_rules! name {
        ($x:expr, $k:expr) => {
            // (ab)using rustc's implementation of closure types
            // the compiler generates a unique type for every closure
            // thus if we pass a new closure every time, then Name is given a unique type every time as well
            // here "||{}" is a closure that takes no arguments and does nothing
            $crate::named::name(||{}, $x, $k)
        }
    }
}

mod listutils {
    use core::cmp::Ordering;
    pub fn merge_by<A, Comp: Fn(&A, &A) -> Ordering>(comp: Comp, mut xs: Vec<A>, mut ys: Vec<A>) -> Vec<A> {
        if xs.is_empty() { return ys; }
        if ys.is_empty() { return xs; }
        match comp(&xs[xs.len() - 1], &ys[ys.len() - 1]) {
            Ordering::Greater => {
                let x = xs.pop().unwrap();
                let mut ret = merge_by(comp, xs, ys);
                ret.push(x);
                ret
            },
            _ => {
                let y = ys.pop().unwrap();
                let mut ret = merge_by(comp, xs, ys);
                ret.push(y);
                ret
            }
        }
    }
    
    pub fn greater_than<A: Ord>(x: &A, y: &A) -> Ordering {
        x.cmp(y)
    }
}

mod sorted {
    use super::named::Named;
    use super::listutils as U;
    use core::marker::PhantomData;
    use core::cmp::Ordering;
    
    #[derive(Clone)]
    pub struct SortedBy<Comp, A>(pub A, PhantomData<Comp>);
    
    pub fn sort_by<Comp, A, CompFn: Fn(&A, &A) -> Ordering>(comp: Named<Comp, CompFn>, mut xs: Vec<A>) -> SortedBy<Comp, Vec<A>> {
        SortedBy({ xs.sort_unstable_by(comp.0); xs }, PhantomData)
    }
    
    pub fn merge_by<Comp, A, CompFn: Fn(&A, &A) -> Ordering>(comp: Named<Comp, CompFn>, xs: SortedBy<Comp, Vec<A>>, ys: SortedBy<Comp, Vec<A>>) -> SortedBy<Comp, Vec<A>> {
        SortedBy(U::merge_by(comp.0, xs.0, ys.0), PhantomData)
    }
}

use sorted::*;
use listutils::greater_than;

fn main() {
    name!(greater_than, |gt| {
        let xs = sort_by(gt.clone(), vec![6, 2, 4]);
        let ys = sort_by(gt.clone(), vec![5, 1, 4]);
        println!("{:?}", merge_by(gt, xs, ys).0);
    });
    
    fn less_than<A: Ord>(x: &A, y: &A) -> core::cmp::Ordering { greater_than(x, y).reverse() }
    name!(greater_than, |up| {
        name!(less_than, |down| {
            let xs = sort_by(up.clone(), vec![6, 2, 4]);
            let ys = sort_by(down.clone(), vec![5, 1, 4]);
            // this will not compile
            // println!("{:?}", merge_by(up, xs, ys).0);
        })
    });
}
