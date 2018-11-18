#include "QuotesData.h"



QuotesData::QuotesData(QObject *parent)
	:	QObject(parent),
		aggregatedQuotesMap(),
		snapshot(),
		snapshotTotalQuotes(),
		snapshotVolumeWeightedAveragePrice(),
		mutex()
{
}



QuotesData::~QuotesData()
{
}



void QuotesData::makeSnapshot() const
{
	QMutexLocker locker(&mutex);

	snapshot.clear();

	typedef QMap<int, QList<int>>::const_iterator PriceIter;
	typedef QList<int>::const_iterator VolumeIter;

	long long priceVolumeAccum = 0;
	long long volumeAccum = 0;

	snapshotTotalQuotes = 0;

	for (PriceIter priceIter = aggregatedQuotesMap.constBegin(); priceIter != aggregatedQuotesMap.constEnd(); ++priceIter) {
		int price = priceIter.key();
		const QList<int> &volumes = priceIter.value();
		int sumVolume = 0;
		for (VolumeIter volumeIter = volumes.constBegin(); volumeIter != volumes.constEnd(); ++volumeIter) {
			int volume = *volumeIter;
			sumVolume += *volumeIter;
			priceVolumeAccum += price * volume;
		}
		snapshot.push_back(Quote(price, sumVolume));
		snapshotTotalQuotes += volumes.size();
		volumeAccum += sumVolume;
	}

	snapshotVolumeWeightedAveragePrice = volumeAccum ? priceVolumeAccum / volumeAccum : 0;
}



void QuotesData::updateQuotes(QList<Quote> *currentQuotes, QList<Quote> *previousQuotes)
{
	QMutexLocker locker(&mutex);

	typedef QList<Quote>::const_iterator QuoteIter;

	if (previousQuotes != 0) {
		for (QuoteIter quote = previousQuotes->constBegin(); quote != previousQuotes->constEnd(); ++quote) {
			int price = quote->first;
			int volume = quote->second;
			if (aggregatedQuotesMap.contains(price)) {
				QList<int> &volumes = aggregatedQuotesMap[price];
				if (volumes.contains(volume)) {
					volumes.removeOne(volume);
				}
				if (volumes.empty()) {
					aggregatedQuotesMap.remove(price);
				}
			}
		}
	}

	if (currentQuotes != 0) {
		for (QuoteIter quote = currentQuotes->constBegin(); quote != currentQuotes->constEnd(); ++quote) {
			int price = quote->first;
			int volume = quote->second;
			if (aggregatedQuotesMap.contains(price)) {
				aggregatedQuotesMap[price].push_back(volume);
			}
			else {
				QList<int> volumes;
				volumes.push_back(volume);
				aggregatedQuotesMap.insert(price, volumes);
			}
		}
	}

	emit dataChanged();
}



int QuotesData::getSnapshotVolumeWeightedAveragePrice() const
{
	return snapshotVolumeWeightedAveragePrice;
}



int QuotesData::getSnapshotAggregatedListSize() const
{
	return snapshot.size();
}



int QuotesData::getSnapshotTotalListSize() const
{
	return snapshotTotalQuotes;
}



int QuotesData::getSnapshotData(int row, int column) const
{
	if (snapshot.size() <= row || 2 <= column) {
		return 0;
	}

	return column ? snapshot[row].second : snapshot[row].first;
}
