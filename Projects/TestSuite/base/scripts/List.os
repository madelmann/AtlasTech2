
public prototype List<T>
{
	private T mFirst;
	private T mLast;
	private Number mSize;

	public Void List() {
		mFirst = T();
		mLast = T();
		mSize = 0;
	}

	public T at(Number index) {
		return mLast;
	}

	public Boolean empty() const {
		return mSize == 0;
	}

	public Number size() const {
		return mSize;
	}

	public Void push_back(T t) {
		mLast = t;
	}
	
	public Void push_front(T t) {
		mFirst = t;
	}

	public Void pop_back() {
	}
	
	public Void pop_front() {
	}
}


private object Main
{
	public Number Main() {
		List<Number> numbers;

		numbers.push_back(1);
		numbers.push_back(2);

		print("numbers.size() = " & numbers.size());

		Number num1 = numbers.at(1);
		print("num1 = " & num1);

		numbers.pop_back();
		print("numbers.size() = " & numbers.size());
	}
}